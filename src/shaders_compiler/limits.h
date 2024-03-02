#pragma once

constexpr size_t RESOURCE_MAX_NAME_LEN = 64;

constexpr size_t MAX_REGISTER = 32;
constexpr size_t TEXTURE_REGISTERS_BEGIN = 0;
constexpr size_t SAMPLER_REGISTERS_BEGIN = TEXTURE_REGISTERS_BEGIN + MAX_REGISTER;
constexpr size_t BUFFER_REGISTERS_BEGIN = SAMPLER_REGISTERS_BEGIN + MAX_REGISTER;

// Vulkan requires a variable sized array to be set in the last binding of the descriptor set
constexpr size_t VARIABLE_SIZE_ARRAY_REGISTER = BUFFER_REGISTERS_BEGIN + MAX_REGISTER;
