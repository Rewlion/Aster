#pragma once

#include <engine/assert.h>

namespace Utils
{
  template<size_t N>
  class BitCapacity
  {
    static constexpr size_t batchSize = sizeof(uint64_t);
    static constexpr size_t batchesCount = (N+batchSize) / batchSize;
    using BatchType = uint64_t;

    public:
      inline void ResetAll()
      {
        for (size_t i = 0; i < batchesCount; ++i)
          batches[i] = (BatchType)(~0);
      }

      inline void Set(const size_t i)
      {
        ASSERT(i < N);
        BatchType& batch = GetBatch(i);
        const size_t j = GetPosInBatch(i);
        batch |= 0x1ull << j;
      }

      inline void Reset(const size_t i)
      {
        ASSERT(i < N);
        BatchType& batch = GetBatch(i);
        const size_t j = GetPosInBatch(i);
        batch &= ~(0x1ull << j);
      }

      inline bool IsSet(const size_t i)
      {
        ASSERT(i < N);
        BatchType& batch = GetBatch(i);
        const size_t j = GetPosInBatch(i);
        size_t bit = 0x1ull & (batch >> j);

        return bit == 1;
      }

    private:
      BatchType& GetBatch(const size_t i)
      {
        const size_t batchId = (i+batchSize)/batchSize;
        return batches[batchId];
      }

      size_t GetPosInBatch(const size_t i)
      {
        return i % batchSize;
      }

    private:
      BatchType batches[batchesCount];
  };
}
