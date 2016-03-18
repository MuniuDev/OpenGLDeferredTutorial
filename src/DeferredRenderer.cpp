/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#include <Renderer.hpp>

DeferredRenderer::DeferredRenderer(std::shared_ptr<Scene> scene)
  : Renderer(scene) {
  LOGD("Created deferred renderer");

  type = RendererType::DEFERRED;

  m_shader = std::make_shared<ShaderProgram>("res/d_geometry.vsh", "res/d_geometry.fsh");

  m_shader->RegisterUniform("u_mvp");
  m_shader->RegisterUniform("u_transform");
}

DeferredRenderer::~DeferredRenderer() {

}

void DeferredRenderer::InitRenderer(float width, float height) {

  for (auto &mesh : m_scene->m_meshes)
  { mesh->SetShader(m_shader); }

  m_width = width;
  m_height = height;

  ResetBuffers();
}

void DeferredRenderer::RenderScene(float dt) {

  GeometryPass(dt);

  LightPass(dt);
}

void DeferredRenderer::GeometryPass(float dt) {

  m_gbuffer->BindForWriting();

  m_shader->BindProgram();
  m_shader->SetUniform("u_mvp", m_scene->GetCamera()->GetMVP());

  for (auto &mesh : m_scene->m_meshes)
  { mesh->Draw(dt); }
}

void DeferredRenderer::LightPass(float dt) {

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  m_gbuffer->BindForReading();

  m_gbuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
  glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width / 2.0f, m_height / 2.0f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  m_gbuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
  glBlitFramebuffer(0, 0, m_width, m_height, 0, m_height / 2.0f, m_width / 2.0f, m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  m_gbuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  glBlitFramebuffer(0, 0, m_width, m_height, m_width / 2.0f, m_height / 2.0f, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  m_gbuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
  glBlitFramebuffer(0, 0, m_width, m_height, m_width / 2.0f, 0, m_width, m_height / 2.0f, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void DeferredRenderer::Resize(float width, float height) {
  m_width = width;
  m_height = height;

  ResetBuffers();
}

void DeferredRenderer::ResetBuffers() {
  m_gbuffer.reset();

  m_gbuffer = std::make_shared<GBuffer>();
  m_gbuffer->Init(m_width, m_height);
}
