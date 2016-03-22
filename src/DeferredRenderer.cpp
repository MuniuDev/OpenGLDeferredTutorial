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

  m_geometryShader = std::make_shared<ShaderProgram>("res/d_geometry.vsh", "res/d_geometry.fsh");
  m_geometryShader->BindProgram();
  m_geometryShader->RegisterUniform("u_mvp");
  m_geometryShader->RegisterUniform("u_transform");

  m_geometryShader->RegisterUniform("u_material.specularIntensity");
  m_geometryShader->RegisterUniform("u_material.specularPower");
  m_geometryShader->RegisterUniform("u_material.specularColor");

  m_lightShader = std::make_shared<ShaderProgram>("res/d_light.vsh", "res/d_light.fsh");
  m_lightShader->BindProgram();
  m_lightShader->RegisterUniform("u_mvp");
  m_lightShader->RegisterUniform("u_transform");
  m_lightShader->RegisterUniform("u_screenSize");

  m_lightShader->RegisterUniform("gPositionMap");
  m_lightShader->RegisterUniform("gColorMap");
  m_lightShader->RegisterUniform("gNormalMap");
  m_lightShader->RegisterUniform("gSpecularColorMap");
  m_lightShader->RegisterUniform("gSpecularDataMap");

  m_lightShader->RegisterUniform("u_ambientLight.color");
  m_lightShader->RegisterUniform("u_ambientLight.intensity");

  m_lightShader->RegisterUniform("u_eyePos");

  m_lightShader->RegisterUniform("u_directionalLight.base.color");
  m_lightShader->RegisterUniform("u_directionalLight.base.intensity");
  m_lightShader->RegisterUniform("u_directionalLight.direction");

  m_lightShader->RegisterUniform("u_lightCount");
  for (int i = 0; i < MAX_POINT_LIGHT; ++i) {
    m_lightShader->RegisterUniform(ArrayUniformName("u_pointLights", "base.color", i));
    m_lightShader->RegisterUniform(ArrayUniformName("u_pointLights", "base.intensity", i));
    m_lightShader->RegisterUniform(ArrayUniformName("u_pointLights", "position", i));
    m_lightShader->RegisterUniform(ArrayUniformName("u_pointLights", "range", i));
    m_lightShader->RegisterUniform(ArrayUniformName("u_pointLights", "attenuation", i));
  }

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

  m_lightShader->BindProgram();

  int lightCount = glm::min((int)m_scene->m_pointLights.size(), MAX_POINT_LIGHT);

  m_lightShader->SetUniform("u_lightCount", lightCount);
  m_lightShader->SetUniform("u_screenSize", glm::vec2(m_width, m_height));

  for (int i = 0; i < lightCount; ++i) {
    PointLight light = m_scene->m_pointLights[i];
    m_lightShader->SetUniform(ArrayUniformName("u_pointLights", "base.color", i), light.base.color);
    m_lightShader->SetUniform(ArrayUniformName("u_pointLights", "base.intensity", i), light.base.intensity);
    m_lightShader->SetUniform(ArrayUniformName("u_pointLights", "position", i), light.position);
    m_lightShader->SetUniform(ArrayUniformName("u_pointLights", "range", i), light.range);
    m_lightShader->SetUniform(ArrayUniformName("u_pointLights", "attenuation", i), light.attenuation);
  }

  m_lightShader->SetUniform("gPositionMap", (int) GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
  m_lightShader->SetUniform("gColorMap", (int) GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
  m_lightShader->SetUniform("gNormalMap", (int) GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  m_lightShader->SetUniform("gSpecularColorMap", (int)GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR_COLOR);
  m_lightShader->SetUniform("gSpecularDataMap", (int)GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR_DATA);

  m_lightShader->SetUniform("u_ambientLight.color", m_scene->m_ambientLight.color);
  m_lightShader->SetUniform("u_ambientLight.intensity", m_scene->m_ambientLight.intensity);

  m_lightShader->SetUniform("u_directionalLight.base.color", m_scene->m_directionalLight.base.color);
  m_lightShader->SetUniform("u_directionalLight.base.intensity", m_scene->m_directionalLight.base.intensity);
  m_lightShader->SetUniform("u_directionalLight.direction", m_scene->m_directionalLight.direction);

  m_lightShader->SetUniform("u_mvp", glm::mat4(1.0f));
  m_lightShader->SetUniform("u_transform", glm::mat4(1.0f));

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

  m_geometryShader->BindProgram();
  m_geometryShader->SetUniform("u_mvp", m_scene->GetCamera()->GetMVP());
  for (auto &mesh : m_scene->m_meshes)
    mesh->Draw(dt);

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

  m_lightShader->BindProgram();

  m_lightShader->SetUniform("u_eyePos", m_scene->GetCamera()->GetPos());

  //in future, should be replaced by proper multipass light rendering
  quad->Render();

  //deinit
  glDisable(GL_BLEND);
}

void DeferredRenderer::Resize(float width, float height) {
  m_width = width;
  m_height = height;

  m_lightShader->BindProgram();
  m_lightShader->SetUniform("u_screenSize", glm::vec2(m_width, m_height));

  ResetBuffers();
}

void DeferredRenderer::ResetBuffers() {
  m_gbuffer.reset();

  m_gbuffer = std::make_shared<GBuffer>();
  m_gbuffer->Init((unsigned int)m_width, (unsigned int)m_height);
}
