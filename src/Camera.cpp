/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#include <Camera.hpp>

#include <Input.hpp>


static const float MOVE_SPEED = 100.0f;
static const float ROT_SPEED = 0.5f;


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
  { pos = glm::vec3(-1, 0, 0); }
  else if (g_input.GetKeyState(SDL_SCANCODE_D))
  { pos = glm::vec3( 1, 0, 0); }

  if (g_input.GetKeyState(SDL_SCANCODE_W))
  { pos = glm::vec3(0, 0, -1); }
  else if (g_input.GetKeyState(SDL_SCANCODE_S))
  { pos = glm::vec3(0, 0, 1); }

  if (glm::length(pos) > 0.0f) {
    pos = glm::normalize(pos) * MOVE_SPEED * dt;
    Move(pos);
  }

  if (g_input.IsLMBPressed()) {
    glm::vec2 v = g_input.GetMouseDeltaPos() * ROT_SPEED * dt;
    glm::quat rot = glm::angleAxis(-v.x, glm::vec3(0, 1, 0))
                    * glm::angleAxis(-v.y, GetRight());
    Rotate(rot);
  }
}

glm::mat4 Camera::GetMVP() {
  // translate in opposite direction, hence -m_pos
  glm::mat4 model = glm::translate(glm::mat4(1.0f), -m_pos);
  // rotate in opposite direction, hence conjugation
  glm::mat4 view = glm::mat4_cast(glm::conjugate(m_rot));
  return m_perspective * view * model;
}

void Camera::SetPos(const glm::vec3 &pos) {
  m_pos = pos;
}

void Camera::Move(const glm::vec3 &dir) {
  // move in local coordinates
  m_pos += glm::vec3(m_rot * glm::vec4(dir, 1));
}

void Camera::Rotate(const glm::quat &rot) {
  m_rot = glm::normalize(rot) * m_rot;
}

glm::vec3 Camera::GetUp() {
  return glm::vec3(m_rot * glm::vec4(0, 1, 0, 1));
}

glm::vec3 Camera::GetRight() {
  return glm::vec3(m_rot * glm::vec4(1, 0, 0, 1));
}

glm::vec3 Camera::GetFront() {
  return glm::vec3(m_rot * glm::vec4(0, 0, -1, 1));
}
