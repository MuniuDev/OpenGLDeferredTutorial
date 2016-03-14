/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#pragma once

#include <Common.hpp>

class Input {
 public:
  Input();
  void Update();

  bool GetKeyState(int keycode);
  glm::vec2 GetMousePosition();
  glm::vec2 GetMouseDeltaPos();
  bool IsLMBPressed();

 private:
  Uint8 *m_keystate;

  glm::vec2 m_mousePos;
  glm::vec2 m_prevMousePos;
  bool m_lmbPressed;

};

extern Input g_input;
