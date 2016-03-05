/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#include <Camera.hpp>

#include <Input.hpp>


Camera::Camera(float fov, float aspect, float clipNear, float clipFar) {
  Resize(fov, aspect, clipNear, clipFar);
}

void Camera::Resize(float fov, float aspect, float clipNear, float clipFar) {
  m_fov = fov;
  m_aspect = aspect;
  m_clipNear = clipNear;
  m_clipFar = clipFar;
  m_perspective = glm::perspective(glm::radians(45.0f), m_aspect,
                                   m_clipNear, m_clipFar);
}

void Camera::HandleInput(float dt) {
  glm::vec3 pos;

  if (g_input.GetKeyState(SDL_SCANCODE_A))
  { pos = -GetRight(); }
  else if (g_input.GetKeyState(SDL_SCANCODE_D))
  { pos = GetRight(); }

  if (g_input.GetKeyState(SDL_SCANCODE_W))
  { pos = -GetFront(); }
  else if (g_input.GetKeyState(SDL_SCANCODE_S))
  { pos = GetFront(); }

  pos *= dt;
  Move(pos);

  if (g_input.IsLMBPressed()) {
    glm::vec2 v = g_input.GetMouseDeltaPos();
    glm::vec3 direction = glm::vec3(v.y, v.x, 0.0f) * dt * 10.0f;
    glm::quat rot = glm::angleAxis(0.01f, direction);
    Rotate(rot);
  }
}

glm::mat4 Camera::GetMVP() {
  glm::mat4 mat = glm::translate(glm::mat4(1.0f), m_pos * -1.0f);
  return m_perspective * glm::mat4_cast(m_rot) * mat;
}

void Camera::SetPos(const glm::vec3 &pos) {
  m_pos = pos;
}

void Camera::Move(const glm::vec3 &pos) {
  m_pos += glm::vec3(glm::mat4_cast(m_rot) * glm::vec4(pos, 1));
}

void Camera::Rotate(const glm::quat &rot) {
  m_rot = rot * m_rot;
}

glm::vec3 Camera::GetUp() {
  return glm::vec3(glm::mat4_cast(m_rot) * glm::vec4(0, 1, 0, 1));
}

glm::vec3 Camera::GetRight() {
  return glm::vec3(glm::mat4_cast(m_rot) * glm::vec4(1, 0, 0, 1));
}

glm::vec3 Camera::GetFront() {
  return glm::vec3(glm::mat4_cast(m_rot) * glm::vec4(0, 0, 1, 1));
}
