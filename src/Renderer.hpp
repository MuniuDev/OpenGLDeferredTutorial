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
  Renderer() { }
  virtual ~Renderer() { }

  virtual void InitRenderer(std::shared_ptr<Scene> scene, float, float) = 0;
  virtual void RenderScene(float dt) = 0;
  virtual void Resize(float, float) = 0;

  RendererType GetType() {
    return type;
  }

 protected:
  std::shared_ptr<Scene> m_scene;

  RendererType type;
};


class ForwardRenderer : public Renderer {
 public:
  ForwardRenderer();
  ~ForwardRenderer();

  void InitRenderer(std::shared_ptr<Scene> scene, float, float);
  void RenderScene(float dt);
  void Resize(float, float);

 private:
  std::shared_ptr<ShaderProgram> m_shader;
};

class DeferredRenderer : public Renderer {
 public:
  DeferredRenderer();
  ~DeferredRenderer();

  void InitRenderer(std::shared_ptr<Scene> scene, float width, float height);
  void RenderScene(float dt);

  void Resize(float width, float height);

 private:
  void GeometryPass(float dt);
  void LightPass(float dt);

 protected:
  std::shared_ptr<GBuffer> m_gbuffer;
  float m_width;
  float m_height;

  void ResetBuffers();

 private:
  std::shared_ptr<ShaderProgram> m_geometryShader;
  std::shared_ptr<ShaderProgram> m_lightShader;
};
