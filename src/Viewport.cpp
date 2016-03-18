/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#include <Viewport.hpp>

#include <Input.hpp>
//#include <Mesh.hpp>

Viewport::Viewport(float width, float height)
  : m_width(width),
    m_height(height),
    m_camera(nullptr) {

}

void Viewport::Resize(float width, float height) {
  LOGD("Resizing viewport to {}x{}", width, height);
  m_width = width;
  m_height = height;
  m_camera->Resize(45.0f, m_width / m_height, 0.1f, 1000.0f);
}

void Viewport::Init() {

  m_camera = std::make_shared<Camera>(45.0f, m_width / m_height, 0.1f, 1000.0f);

  m_scene = std::make_shared<Scene>();
  m_scene->Init(m_camera);

  m_deferredRenderer = std::make_shared<DeferredRenderer>(m_scene);
  m_forwardRenderer = std::make_shared<ForwardRenderer>(m_scene);

  m_renderer = m_forwardRenderer;
  m_renderer->InitRenderer(m_width, m_height);
}

void Viewport::Draw(float dt) {

  m_scene->Update(dt);

  if (g_input.GetKeyState(SDL_SCANCODE_F)) {
    SetActiveRenderer(RendererType::FORWARD);
  } else if (g_input.GetKeyState(SDL_SCANCODE_R)) {
    SetActiveRenderer(RendererType::DEFERRED);
  }

  glViewport(0, 0, m_width, m_height);

  m_renderer->RenderScene(dt);
}

void Viewport::SetActiveRenderer(RendererType type) {

  if (type != m_renderer->GetType()) {
    switch (type) {
      case RendererType::FORWARD:
        m_renderer = m_forwardRenderer;
        LOGD("Changed renderer to forward");
        break;
      case RendererType::DEFERRED:
        m_renderer = m_deferredRenderer;
        LOGD("Changed renderer to deferred");
        break;
      default:
        break;
    }

    m_renderer->InitRenderer(m_width, m_height);
  }
}
