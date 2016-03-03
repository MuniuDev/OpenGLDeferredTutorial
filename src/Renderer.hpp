/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#pragma once

#include <Common.hpp>
#include <Viewport.hpp>

#include <vector>

class RendererContext {
 public:
  RendererContext(float width, float height, std::string name);
  virtual ~RendererContext();

  void InitGL();
  void Tic();
  void HandleWindowEvent(const SDL_Event &event);

 private:
  void Clear();
  void Draw();
  void SwapBuffers();

  float m_width;
  float m_height;

  // Viewports
  Viewport m_viewport;

  //SDL
  SDL_Window *m_window;
  SDL_GLContext m_context;

};
