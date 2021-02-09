#include <benchmark/benchmark.h>

#include <engine/utils/mesh_generation.h>

void BM_SphereMeshGeneration(benchmark::State& state)
{
  for (auto _ : state)
  {
    auto [vertices, indices] = Utils::GenerateSphere(6, 1.0f);
    (void)vertices;
    (void)indices;
  }
}

BENCHMARK(BM_SphereMeshGeneration)->Unit(benchmark::kSecond);

BENCHMARK_MAIN();