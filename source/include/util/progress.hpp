#pragma once

#include "thread/spinlock.hpp"
#include <cstddef>

class Progress {
public:
  Progress(size_t total, size_t step = 1);

  void Update(size_t count);

private:
  size_t m_Total;
  size_t m_Current;

  size_t m_Percent, m_LastPercent, m_Step;

  SpinLock m_SpinLock;
};