/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#include <Renderer.hpp>

#include <sstream>
#include <cmath>

#define MAX_POINT_LIGHT 10

std::string ArrayUniformName(const std::string &name, const std::string &field, int idx) {
  std::stringstream ss("");
  ss << name;
  ss << "[";
  ss << idx;
  ss << "].";
  ss << field;
  return ss.str();
}

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
  m_shader->RegisterUniform("u_material.specularColor");

  m_shader->RegisterUniform("u_directionalLight.base.color");
  m_shader->RegisterUniform("u_directionalLight.base.intensity");
  m_shader->RegisterUniform("u_directionalLight.direction");

  m_shader->RegisterUniform("u_lightCount");
  for (int i = 0; i < MAX_POINT_LIGHT; ++i) {
    m_shader->RegisterUniform(ArrayUniformName("u_pointLights", "base.color", i));
    m_shader->RegisterUniform(ArrayUniformName("u_pointLights", "base.intensity", i));
    m_shader->RegisterUniform(ArrayUniformName("u_pointLights", "position", i));
    m_shader->RegisterUniform(ArrayUniformName("u_pointLights", "range", i));
    m_shader->RegisterUniform(ArrayUniformName("u_pointLights", "attenuation", i));
  }
}

ForwardRenderer::~ForwardRenderer() {

}

void ForwardRenderer::InitRenderer(float, float) {

  for (auto &mesh : m_scene->m_meshes)
  { mesh->SetShader(m_shader); }

}

void ForwardRenderer::RenderScene(float dt) {
  m_shader->BindProgram();

  m_shader->SetUniform("u_ambientLight.color", m_scene->m_ambientLight.color);
  m_shader->SetUniform("u_ambientLight.intensity", m_scene->m_ambientLight.intensity);

  m_shader->SetUniform("u_directionalLight.base.color", m_scene->m_directionalLight.base.color);
  m_shader->SetUniform("u_directionalLight.base.intensity", m_scene->m_directionalLight.base.intensity);
  m_shader->SetUniform("u_directionalLight.direction", m_scene->m_directionalLight.direction);

  m_shader->SetUniform("u_eyePos", m_scene->GetCamera()->GetPos());

  m_shader->SetUniform("u_mvp", m_scene->GetCamera()->GetMVP());

  int lightCount = glm::min((int)m_scene->m_pointLights.size(), MAX_POINT_LIGHT);

  m_shader->SetUniform("u_lightCount", lightCount);

  for (int i = 0; i < lightCount; ++i ) {
    PointLight light = m_scene->m_pointLights[i];
    m_shader->SetUniform(ArrayUniformName("u_pointLights", "base.color", i), light.base.color);
    m_shader->SetUniform(ArrayUniformName("u_pointLights", "base.intensity", i), light.base.intensity);
    m_shader->SetUniform(ArrayUniformName("u_pointLights", "position", i), light.position);
    m_shader->SetUniform(ArrayUniformName("u_pointLights", "range", i), light.range);
    m_shader->SetUniform(ArrayUniformName("u_pointLights", "attenuation", i), light.attenuation);
  }

  for (auto &mesh : m_scene->m_meshes)
  { mesh->Draw(dt); }

  //unbind shader
  glUseProgram(0);
}

void ForwardRenderer::Resize(float, float) {

}
