/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#include <Camera.hpp>

Camera::Camera(float fov, float aspect, float clipNear, float clipFar) {
  Resize(fov, aspect, clipNear, clipFar);
}

void Camera::Resize(float fov, float aspect, float clipNear, float clipFar) {
  m_fov = fov;
  m_aspect = aspect;
  m_clipNear = clipNear;
  m_clipFar = clipFar;
  m_perspective = glm::perspective(glm::radians(45.0f), m_aspect, m_clipNear, m_clipFar);
}

void Camera::HandleInput() {
  //TODO
}

glm::mat4 Camera::GetMVP() {
  glm::mat4 mat = glm::translate(glm::mat4(1.0f),m_pos*-1.0f);
  return m_perspective * glm::mat4_cast(m_rot) * mat;
}
