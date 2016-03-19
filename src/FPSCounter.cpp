/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#include <FPSCounter.hpp>

#include <limits>

#define INTERVAL 1.0f

FPSCounter::FPSCounter() {

}

void FPSCounter::Init() {
  m_lastTicTimePoint = std::chrono::system_clock::now();
  m_lastDeltaTime = std::numeric_limits<float>::max();
  m_timeSum = 0.0f;
}

float FPSCounter::TicAndGetDeltaTime() {
  auto tmp = std::chrono::system_clock::now();
  std::chrono::duration<float> deltaTime = tmp - m_lastTicTimePoint;
  m_lastTicTimePoint = tmp;
  m_lastDeltaTime = deltaTime.count();

  m_frameSum += 1;
  m_timeSum += m_lastDeltaTime;

  return m_lastDeltaTime;
}

float FPSCounter::GetFPS() {
  return m_frameSum / m_timeSum;
}

void FPSCounter::Print() {
  // update fps every second
  if (m_timeSum >= INTERVAL) {
    LOGD("FPS: {}", GetFPS());
    m_timeSum -= INTERVAL;
    m_frameSum = 0;
  }
}