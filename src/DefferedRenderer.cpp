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

  m_gbuffer.reset();

  m_gbuffer = std::make_shared<GBuffer>();
  m_gbuffer->Init(width, height);


}

void DeferredRenderer::RenderScene(float dt) {

  GeometryPass(dt);

  LightPass(dt);
}

void DeferredRenderer::GeometryPass(float dt) {
  m_shader->BindProgram();

  m_shader->SetUniform("u_mvp", m_scene->GetCamera()->GetMVP());

  for (auto &mesh : m_scene->m_meshes)
  { mesh->Draw(dt); }
}

void DeferredRenderer::LightPass(float dt) {

}
