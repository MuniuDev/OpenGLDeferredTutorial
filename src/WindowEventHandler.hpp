/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#pragma once

#include <Common.hpp>
#include <Renderer.hpp>

class WindowEventHandler {
public:
  WindowEventHandler(std::shared_ptr<RendererContext> renderer);

  void PollEvents();
  bool ShouldClose();

private:
  std::shared_ptr<RendererContext> m_renderer;
  bool m_shouldClose;
};
