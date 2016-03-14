/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#include <Input.hpp>

Input g_input;

Input::Input() {
}

void Input::Update() {

  m_keystate = const_cast<Uint8 *>(SDL_GetKeyboardState(NULL));

  int x, y;
  SDL_GetMouseState(&x, &y);
  m_mousePos = glm::vec2(x, y);

  bool isPressed = m_lmbPressed;
  m_lmbPressed = SDL_GetMouseState(NULL, NULL)&SDL_BUTTON_LEFT;

  if (isPressed != m_lmbPressed)
  { m_prevMousePos = m_mousePos; }
}

bool Input::GetKeyState(int keycode) {
  return m_keystate[keycode];
}

glm::vec2 Input::GetMousePosition() {
  return m_mousePos;
}

glm::vec2 Input::GetMouseDeltaPos() {

  glm::vec2 delta = m_mousePos - m_prevMousePos;
  m_prevMousePos = m_mousePos;

  return delta;
}

bool Input::IsLMBPressed() {
  return m_lmbPressed;
}
