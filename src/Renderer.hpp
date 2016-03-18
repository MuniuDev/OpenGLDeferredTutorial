/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#pragma once

#include <Common.hpp>
#include <Scene.hpp>
#include <GBuffer.hpp>


enum class RendererType {
  FORWARD,
  DEFERRED
};

class Renderer {
 public:
  Renderer(std::shared_ptr<Scene> scene)
    : m_scene(scene) { }
  virtual ~Renderer() { }

  virtual void InitRenderer(float, float) = 0;
  virtual void RenderScene(float dt) = 0;

  RendererType GetType() {
    return type;
  }

 protected:
  std::shared_ptr<ShaderProgram> m_shader;
  std::shared_ptr<Scene> m_scene;

  RendererType type;
};


class ForwardRenderer : public Renderer {
 public:
  ForwardRenderer(std::shared_ptr<Scene> scene);
  ~ForwardRenderer();

  void InitRenderer(float, float);
  void RenderScene(float dt);
};

class DeferredRenderer : public Renderer {
 public:
  DeferredRenderer(std::shared_ptr<Scene> scene);
  ~DeferredRenderer();

  void InitRenderer(float width, float height);
  void RenderScene(float dt);

  void GeometryPass(float dt);
  void LightPass(float dt);

 protected:
  std::shared_ptr<GBuffer> m_gbuffer;
};
