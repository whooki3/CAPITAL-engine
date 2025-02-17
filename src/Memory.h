#pragma once

#include "vulkan/vulkan.h"

#include <cstring>
#include "array"
#include "vector"

class Memory {
 public:
  Memory();
  ~Memory();

  struct PushConstants {
    VkShaderStageFlags shaderStage = {VK_SHADER_STAGE_COMPUTE_BIT};
    uint32_t offset = 0;
    uint32_t size = 128;
    std::array<uint64_t, 32> data;
  } pushConstants;

  struct Buffers {
    std::vector<VkBuffer> shaderStorage;
    std::vector<VkDeviceMemory> shaderStorageMemory;

    std::vector<VkBuffer> uniforms;
    std::vector<VkDeviceMemory> uniformsMemory;
    std::vector<void*> uniformsMapped;

    struct CommandBuffers {
      VkCommandPool pool;
      std::vector<VkCommandBuffer> graphic;
      std::vector<VkCommandBuffer> compute;
    } command;
  } buffers;

  struct DescriptorSets {
    VkDescriptorPool pool;
    VkDescriptorSetLayout setLayout;
    std::vector<VkDescriptorSet> sets;
  } descriptor;

 public:
  void createFramebuffers();

  void createCommandPool();
  void createCommandBuffers();
  void createComputeCommandBuffers();

  void createDescriptorPool();
  void createDescriptorSetLayout();
  void createDescriptorSets();

  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
  void recordComputeCommandBuffer(VkCommandBuffer commandBuffer);

  void createShaderStorageBuffers();

  void createUniformBuffers();
  void updateUniformBuffer(uint32_t currentImage);

  void createImage(uint32_t width,
                   uint32_t height,
                   VkSampleCountFlagBits numSamples,
                   VkFormat format,
                   VkImageTiling tiling,
                   VkImageUsageFlags usage,
                   VkMemoryPropertyFlags properties,
                   VkImage& image,
                   VkDeviceMemory& imageMemory);

 private:
  void createBuffer(VkDeviceSize size,
                    VkBufferUsageFlags usage,
                    VkMemoryPropertyFlags properties,
                    VkBuffer& buffer,
                    VkDeviceMemory& bufferMemory);
  uint32_t findMemoryType(uint32_t typeFilter,
                          VkMemoryPropertyFlags properties);
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};
