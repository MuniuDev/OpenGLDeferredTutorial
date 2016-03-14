/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#include <WindowEventHandler.hpp>

#include <Input.hpp>

WindowEventHandler::WindowEventHandler(
  std::shared_ptr<Context> renderer)
  : m_renderer(renderer),
    m_shouldClose(false) {
}

void WindowEventHandler::PollEvents() {
  SDL_Event event;
  while (SDL_PollEvent( &event ) != 0) {
    switch (event.type) {
      case SDL_QUIT:
        m_shouldClose = true;
        break;
      case SDL_WINDOWEVENT:
        m_renderer->HandleWindowEvent(event);
        break;
      default:
        break;
    }
  }

  g_input.Update();
}

bool WindowEventHandler::ShouldClose() {
  return m_shouldClose;
}
