/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#include <Viewport.hpp>

#include <Mesh.hpp>

Viewport::Viewport(float width, float height)
  : m_width(width),
    m_height(height),
    m_camera(45.0f, m_width / m_height, 0.1f, 1000.0f),
    m_shader(nullptr) {

}

void Viewport::Resize(float width, float height) {
  LOGD("Resizing viewport to {}x{}", width, height);
  m_width = width;
  m_height = height;
  m_camera.Resize(45.0f, m_width / m_height, 0.1f, 1000.0f);
}

#ifdef __WIN32__
Mesh mesh("res\\model-tank\\", "tank.fbx");
Mesh ground("res\\model-ground\\", "ground.fbx");
#else
Mesh mesh("res/model-tank/", "tank.fbx");
Mesh ground("res/model-ground/", "ground.fbx");
#endif

void Viewport::Init() {
  m_shader = new ShaderProgram("res/forward.vsh", "res/forward.fsh");
  mesh.Init();
  ground.Init();
  //mesh.SetPos(glm::vec3(0,10,0));
  //mesh.SetRot(glm::quat(0.5,1,0,1));
  m_camera.SetPos(glm::vec3(-8.04383, 4.87507, -6.82812));
  m_camera.SetRotate(glm::quat(-0.48016, 0.0973405, 0.854395, 0.173216));

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

void Viewport::Draw(float dt) {
  //input
  m_camera.HandleInput(dt);

  // set correct viewport
  glViewport(0, 0, m_width, m_height);
  // bind shader
  m_shader->BindProgram();
  
  m_shader->SetUniform("u_ambientLight.color", glm::vec3(1, 1, 1));
  m_shader->SetUniform("u_ambientLight.intensity", 0.1f);
  
  m_shader->SetUniform("u_material.specularIntensity",5.0f);
  m_shader->SetUniform("u_material.specularPower",10.0f);

  m_shader->SetUniform("u_directionalLight.base.color", glm::vec3(1, 1, 1));
  m_shader->SetUniform("u_directionalLight.base.intensity", 0.9f);
  m_shader->SetUniform("u_directionalLight.direction", glm::normalize(glm::vec3(1, -1, 1)));
  
  m_shader->SetUniform("u_eyePos", m_camera.GetPos());

  m_shader->SetUniform("u_mvp", m_camera.GetMVP());

  m_shader->SetUniform("u_transform", mesh.GetTransformation());
  mesh.Draw(dt);
  m_shader->SetUniform("u_transform", ground.GetTransformation());
  ground.Draw(dt);

  //unbind shader
  glUseProgram(0);
}
