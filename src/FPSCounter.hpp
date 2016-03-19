/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#pragma once

#include <chrono>

#include <Common.hpp>

class FPSCounter {
 public:
  FPSCounter();

  float TicAndGetDeltaTime();
  float GetFPS();

  void Init();
  void Print();

 private:
  std::chrono::time_point<std::chrono::system_clock> m_lastTicTimePoint;
  float m_lastDeltaTime;

  uint_t m_frameSum;
  float m_timeSum;
};