#ifndef MULTISCALE_MEAN_ESTIMATOR_HLSL
#define MULTISCALE_MEAN_ESTIMATOR_HLSL

//https://media.contentapi.ea.com/content/dam/ea/seed/presentations/2019-ray-tracing-gems-chapter-25-barre-brisebois-et-al.pdf
//2.6 Global Illumination

struct MultiscaleMeanEstimatorData_
{
  float3 mean;
  float vbbr;
  float3 variance;
  float inconsistency;
  float3 shortMean;
  float _p;

  void init()
  {
    mean = float3(0,0,0);
    vbbr = 0;
    variance = float3(0,0,0);
    inconsistency = 1;
    shortMean = float3(0,0,0);
  }
};
  
#ifdef __preshader__

float3 MultiscaleMeanEstimator(float3 y,
  inout MultiscaleMeanEstimatorData_ data,
  float shortWindowBlend = 0.08f)
{
  float3 mean = data.mean;
  float3 shortMean = data.shortMean;
  float vbbr = data.vbbr;
  float3 variance = data.variance;
  float inconsistency = data.inconsistency;
  
  // Suppress fireflies.
  {
    float3 dev = sqrt(max(1e-5, variance));
    float3 highThreshold = 0.1 + shortMean + dev * 8;
    float3 overflow = max(0, y - highThreshold);
    y -= overflow;
  }
  
  float3 delta = y - shortMean;
  shortMean = lerp(shortMean, y, shortWindowBlend);
  float3 delta2 = y - shortMean;
  
  // This should be a longer window than shortWindowBlend to avoid bias
  // from the variance getting smaller when the short-term mean does.
  float varianceBlend = shortWindowBlend * 0.5;
  variance = lerp(variance, delta * delta2, varianceBlend);
  float3 dev = sqrt(max(1e-5, variance));
  
  float3 shortDiff = mean - shortMean;
  
  float relativeDiff = dot( float3(0.299, 0.587, 0.114),
        abs(shortDiff) / max(1e-5, dev) );
  inconsistency = lerp(inconsistency, relativeDiff, 0.08);
  
  float varianceBasedBlendReduction =
        clamp( dot( float3(0.299, 0.587, 0.114),
        0.5 * shortMean / max(1e-5, dev) ), 1.0/32, 1 );
  
  float3 catchUpBlend = clamp(smoothstep(0, 1,
        relativeDiff * max(0.02, inconsistency - 0.2)), 1.0/256, 1);
  catchUpBlend *= vbbr;
  
  vbbr = lerp(vbbr, varianceBasedBlendReduction, 0.1);
  mean = lerp(mean, y, saturate(catchUpBlend));
  
  // Output
  data.mean = mean;
  data.shortMean = shortMean;
  data.vbbr = vbbr;
  data.variance = variance;
  data.inconsistency = inconsistency;
  
  return mean;
}

#endif //__preshader__

struct MultiscaleMeanEstimatorData
{
  float2 El_El2;
  float shortMeanLuminance;
  float inconsistency;
  float3 meanIrradiance;
  float _p1;

  void init()
  {
    El_El2 = float2(0,0);
    shortMeanLuminance = 0;
    inconsistency = 1;
    meanIrradiance = float3(0,0,0);
  }
};

#ifdef __preshader__

// https://github.com/EmbarkStudios/kajiya/tree/restir-meets-surfel

#define MSME_SHORT_MEAN_RATE (1.0 / (5.0))

float3 rgb_to_ycbcr(float3 col) {
    float3x3 m = float3x3(0.2126, 0.7152, 0.0722, -0.1146,-0.3854, 0.5, 0.5,-0.4542,-0.0458);
    return mul(m, col);
}

float3 ycbcr_to_rgb(float3 col) {
    float3x3 m = float3x3(1.0, 0.0, 1.5748, 1.0, -0.1873, -.4681, 1.0, 1.8556, 0.0);
    return mul(m, col);
}

// Rec. 709
float calculate_luma(float3 col) {
    return dot(float3(0.2126, 0.7152, 0.0722), col);
}


float3 MultiscaleMeanEstimator(float3 irradiance_sample, inout MultiscaleMeanEstimatorData data)
{
  const float luminanceSample = calculate_luma(irradiance_sample);
  const float2 El_El2 = lerp(data.El_El2, float2(luminanceSample, luminanceSample*luminanceSample), MSME_SHORT_MEAN_RATE);
  const float shortMeanLuminance = lerp(data.shortMeanLuminance, luminanceSample, MSME_SHORT_MEAN_RATE * 0.5);

  const float luminanceVariance = max(0.0, El_El2.y - El_El2.x * El_El2.x);
  const float luminanceDeviation = sqrt(max(1e-6, luminanceVariance));

  // Suppress fireflies.
  {
    const float3 prevIrradiance = data.meanIrradiance;
    const float3 prevIrradiance_ycbcr = rgb_to_ycbcr(prevIrradiance);

    const float prevLuminanceVariance = max(0.0, data.El_El2.y - data.El_El2.x * data.El_El2.x);
    const float prevLuminanceDeviation = sqrt(max(1e-6, prevLuminanceVariance));

    float highThresholdLuminance = 0.1 + data.shortMeanLuminance + prevLuminanceDeviation*8;

    float3 outliers = max(0.0, irradiance_sample - ycbcr_to_rgb(
      prevIrradiance_ycbcr * (highThresholdLuminance / prevIrradiance_ycbcr.x)
    ));

    irradiance_sample -= outliers;
  }

  const float forecastedLuminance = min(El_El2.x * 1.2, lerp(shortMeanLuminance, El_El2.x, 1.5));
  const float3 prevIrradiance = data.meanIrradiance;
  const float3 prevIrradiance_ycbcr = rgb_to_ycbcr(prevIrradiance);
  const float3 prevIrradianceMSMEClamped = ycbcr_to_rgb(
    sign(prevIrradiance_ycbcr) * clamp(
      abs(prevIrradiance_ycbcr),
      abs(prevIrradiance_ycbcr) * (forecastedLuminance - luminanceDeviation) / max(1e-6, prevIrradiance_ycbcr.x),
      abs(prevIrradiance_ycbcr) * (forecastedLuminance + luminanceDeviation) / max(1e-6, prevIrradiance_ycbcr.x)
    )
  );

  const float blendFactor = 0.03;
  const float3 meanIrradiance = lerp(prevIrradianceMSMEClamped, irradiance_sample, blendFactor);
  
  data.El_El2 = El_El2;
  data.shortMeanLuminance = shortMeanLuminance;
  data.meanIrradiance = meanIrradiance;
  data.inconsistency = lerp(data.inconsistency, (abs(shortMeanLuminance - calculate_luma(meanIrradiance)) / luminanceDeviation), 0.08);

  return data.meanIrradiance;
}

#endif //preshader

#endif