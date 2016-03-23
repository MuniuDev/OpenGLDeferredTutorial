/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#include <Renderer.hpp>
#include <Input.hpp>


#define MAX_POINT_LIGHT 10

DeferredRenderer::DeferredRenderer() {
  LOGD("Created deferred renderer");

  type = RendererType::DEFERRED;
  CHECK_GL_ERR();

  quad = std::make_shared<Quad>();

  //TODO load appropriate shaders and register uniforms

  CHECK_GL_ERR();
}

DeferredRenderer::~DeferredRenderer() {

}

void DeferredRenderer::InitRenderer(std::shared_ptr<Scene> scene, float width, float height) {
  m_scene = scene;

  for (auto &mesh : m_scene->m_meshes)
    mesh->SetShader(m_geometryShader);

  m_width = width;
  m_height = height;

  int lightCount = glm::min((int)m_scene->m_pointLights.size(), MAX_POINT_LIGHT);

  //TODO set uniforms that are constant

  ResetBuffers();
  CHECK_GL_ERR();
}

void DeferredRenderer::RenderScene(float dt) {
  GeometryPass(dt);
  LightPass(dt);

  // if 'v' is pressed, draw the G-buffer
  if (g_input.GetKeyState(SDL_SCANCODE_V))
    m_gbuffer->DebugDraw((GLint)m_width, (GLint)m_height);
}

void DeferredRenderer::GeometryPass(float dt) {

  m_gbuffer->BindForWriting();
  glClearColor(0, 0, 0, 0);
  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  //TODO geometry pass implementation

  glUseProgram(0);
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);
}

void DeferredRenderer::LightPass(float) {
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE);

  m_gbuffer->BindForReading();

  glClear(GL_COLOR_BUFFER_BIT);

  //TODO light pass implementation

  //deinit
  glDisable(GL_BLEND);
}

void DeferredRenderer::Resize(float width, float height) {
  m_width = width;
  m_height = height;

  //TODO update screen size uniform

  ResetBuffers();
}

void DeferredRenderer::ResetBuffers() {
  m_gbuffer.reset();

  m_gbuffer = std::make_shared<GBuffer>();
  m_gbuffer->Init((unsigned int)m_width, (unsigned int)m_height);
}
