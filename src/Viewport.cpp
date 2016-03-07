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
#else
Mesh mesh("res/model-tank/", "tank.fbx");
#endif

void Viewport::Init() {
  m_shader = new ShaderProgram("res/forward.vsh", "res/forward.fsh");
  mesh.Init();
  m_camera.Move(glm::vec3(0, 0, 50));
}

void Viewport::Draw(float dt) {
  //input
  m_camera.HandleInput(dt);

  // set correct viewport
  glViewport(0, 0, m_width, m_height);
  // bind shader
  m_shader->BindProgram();

  GLuint MatrixID = glGetUniformLocation(m_shader->GetProgramHandle(), "u_mvp");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(m_camera.GetMVP()));
  mesh.Draw(dt);

  //unbind shader
  glUseProgram(0);
}
