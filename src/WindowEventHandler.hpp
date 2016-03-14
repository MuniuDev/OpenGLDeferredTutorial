/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#pragma once

#include <Common.hpp>
#include <Context.hpp>

class WindowEventHandler {
 public:
  WindowEventHandler(std::shared_ptr<Context> renderer);

  void PollEvents();
  bool ShouldClose();

 private:
  std::shared_ptr<Context> m_renderer;
  bool m_shouldClose;
};
