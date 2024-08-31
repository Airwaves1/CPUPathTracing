#include "util/progress.hpp"

#include <iostream>
#include <ostream>

Progress::Progress(size_t total, size_t step)
    : m_Total(total), m_Current(0), m_Percent(0), m_LastPercent(-1000),
      m_Step(step) {}

void Progress::Update(size_t count) {
  Guard guard(m_SpinLock);

  m_Current += count;

  m_Percent =
      static_cast<double>(m_Current) / static_cast<double>(m_Total) * 100;

  if (m_Percent - m_LastPercent >= m_Step) {
    m_LastPercent = m_Percent;

    std::cout << "\rProgress: " << m_Percent << "%" <<std::flush;
  }
  if (m_Percent == 100) {
    std::cout << std::endl;
  }
}