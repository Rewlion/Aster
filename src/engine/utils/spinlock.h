#pragma once

#include <atomic>

//https://rigtorp.se/spinlock/

namespace Utils
{
  class Spinlock final
  {
    public:
      void lock() noexcept 
      {
        for (;;)
        {
          // Optimistically assume the lock is free on the first try
          if (!m_Lock.exchange(true, std::memory_order_acquire)) {
            return;
          }
          // Wait for lock to be released without generating cache misses
          while (m_Lock.load(std::memory_order_relaxed)) {
            // Issue X86 PAUSE or ARM YIELD instruction to reduce contention between
            // hyper-threads
            __builtin_ia32_pause();
          }
        }
      }

      bool tryLock() noexcept
      {
        // First do a relaxed load to check if lock is free in order to prevent
        // unnecessary cache misses if someone does while(!try_lock())
        return !m_Lock.load(std::memory_order_relaxed) &&
               !m_Lock.exchange(true, std::memory_order_acquire);
      }

      void unlock() noexcept
      {
        m_Lock.store(false, std::memory_order_release);
      }
    private:
      std::atomic<bool> m_Lock = {0};
  };

  class ScopeSpinlock final
  {
    public:
      ScopeSpinlock(Spinlock& lock)
        : m_Lock(lock)
      {
        m_Lock.lock();
      }

      ~ScopeSpinlock()
      {
        m_Lock.unlock();
      }

    private:
      Spinlock& m_Lock;
  };
}
