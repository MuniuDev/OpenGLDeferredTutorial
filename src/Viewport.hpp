/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#pragma once

#include <memory>
#include <unordered_map>

#include <Common.hpp>
#include <ShaderProgram.hpp>
#include <Camera.hpp>
#include <Renderer.hpp>
#include <Scene.hpp>

class Viewport {
 public:
  Viewport(float width, float height);

  void Init();
  void Resize(float width, float height);
  void Draw(float dt);

  void SetActiveRenderer(RendererType type);

 private:
  float m_width;
  float m_height;

  std::shared_ptr<Scene> m_scene;
  std::shared_ptr<Camera> m_camera;

  std::shared_ptr<Renderer> m_renderer; //current renderer

  std::shared_ptr<DeferredRenderer> m_deferredRenderer;
  std::shared_ptr<ForwardRenderer> m_forwardRenderer;
};
