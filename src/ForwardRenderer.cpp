/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#include <Renderer.hpp>

ForwardRenderer::ForwardRenderer(std::shared_ptr<Scene> scene)
  : Renderer(scene) {
  LOGD("Created forward renderer");

  type = RendererType::FORWARD;

  m_shader = std::make_shared<ShaderProgram>("res/forward.vsh", "res/forward.fsh");

  m_shader->RegisterUniform("u_mvp");
  m_shader->RegisterUniform("u_transform");

  m_shader->RegisterUniform("u_ambientLight.color");
  m_shader->RegisterUniform("u_ambientLight.intensity");

  m_shader->RegisterUniform("u_eyePos");

  m_shader->RegisterUniform("u_material.specularIntensity");
  m_shader->RegisterUniform("u_material.specularPower");

  m_shader->RegisterUniform("u_directionalLight.base.color");
  m_shader->RegisterUniform("u_directionalLight.base.intensity");
  m_shader->RegisterUniform("u_directionalLight.direction");
}

ForwardRenderer::~ForwardRenderer() {

}

void ForwardRenderer::InitRenderer() {

  for (auto &mesh : m_scene->m_meshes)
  { mesh->SetShader(m_shader); }

}

void ForwardRenderer::RenderScene(float dt) {
  m_shader->BindProgram();

  m_shader->SetUniform("u_ambientLight.color", glm::vec3(1, 1, 1));
  m_shader->SetUniform("u_ambientLight.intensity", 0.1f);

  m_shader->SetUniform("u_directionalLight.base.color", glm::vec3(1, 1, 1));
  m_shader->SetUniform("u_directionalLight.base.intensity", 0.9f);
  m_shader->SetUniform("u_directionalLight.direction", glm::normalize(glm::vec3(1, -1, 1)));

  m_shader->SetUniform("u_eyePos", m_scene->GetCamera()->GetPos());

  m_shader->SetUniform("u_mvp", m_scene->GetCamera()->GetMVP());

  for (auto &mesh : m_scene->m_meshes)
  { mesh->Draw(dt); }

  //unbind shader
  glUseProgram(0);
}
