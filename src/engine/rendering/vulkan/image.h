#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>

#include <tuple>

namespace Vulkan
{
  class ImageView
  {
  public:
    ImageView() = default;

    ImageView(vk::Image image, vk::ImageView view, const vk::DescriptorImageInfo& descriptorImageInfo, const vk::ImageSubresourceRange& subresourceRange)
      : image(image)
      , view(view)
      , descriptorImageInfo(descriptorImageInfo)
      , subresourceRange(subresourceRange)
    {

    }

    inline vk::ImageView Get() const
    {
      return view;
    }

    // HAAAAAAAAAAAACKS
    inline vk::Image GetImage() const
    {
      return image;
    }

    inline const vk::DescriptorImageInfo& GetDescriptorImageInfo() const
    {
      return descriptorImageInfo;
    }

    inline const vk::ImageSubresourceRange& GetSubresourceRange() const
    {
      return subresourceRange;
    }

    inline bool operator<(const ImageView& r) const
    {
      return view < r.view;
    }

  private:
    vk::Image image;
    vk::ImageView view;
    vk::DescriptorImageInfo descriptorImageInfo;
    vk::ImageSubresourceRange subresourceRange;
  };

  class Image
  {
  public:
    Image() = default;

    Image(const vk::Device& logicalDevice, const vk::Image& img, vk::Format format, vk::Extent2D extent)
    {
      this->swapchainImage = img;

      subresourceRange = vk::ImageSubresourceRange()
        .setAspectMask(vk::ImageAspectFlagBits::eColor)
        .setBaseMipLevel(0)
        .setLevelCount(1)
        .setBaseArrayLayer(0)
        .setLayerCount(1);

      const auto viewCreateInfo = vk::ImageViewCreateInfo()
        .setImage(img)
        .setViewType(vk::ImageViewType::e2D)
        .setFormat(format)
        .setSubresourceRange(subresourceRange);

      view = logicalDevice.createImageViewUnique(viewCreateInfo);
    }

    Image(vk::UniqueImage img, vk::UniqueImageView view, vk::UniqueDeviceMemory memory, vk::UniqueSampler sampler)
      : image(std::move(img))
      , view(std::move(view))
      , memory(std::move(memory))
      , sampler(std::move(sampler))
    {
      descriptorImageInfo
        .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
        .setImageView(this->view.get())
        .setSampler(this->sampler.get());
    }

    inline ImageView GetView() const
    {
      return ImageView(image.get(), view.get(), descriptorImageInfo, subresourceRange);
    }

    inline vk::Image GetImage() const
    {
      return image.get();
    }

    inline vk::DeviceMemory GetMemory() const
    {
      return memory.get();
    }

    inline vk::Sampler GetSampler() const
    {
      return sampler.get();
    }

    inline const vk::DescriptorImageInfo& GetDescriptorImageInfo() const
    {
      return descriptorImageInfo;
    }

  private:
    vk::Image swapchainImage;
    vk::UniqueImage image;
    vk::UniqueImageView view;
    vk::UniqueDeviceMemory memory;
    vk::UniqueSampler sampler;
    vk::DescriptorImageInfo descriptorImageInfo;
    vk::ImageSubresourceRange subresourceRange;
  };

}