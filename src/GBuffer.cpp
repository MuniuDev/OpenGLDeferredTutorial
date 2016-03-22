/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#include <GBuffer.hpp>

GBuffer::GBuffer() {
  m_fbo = 0;
  m_depthTexture = 0;
  memset(m_textures, 0, sizeof(m_textures));
}

GBuffer::~GBuffer() {
  if (m_fbo != 0) {
    glDeleteFramebuffers(1, &m_fbo);
  }

  if (m_textures[0] != 0) {
    glDeleteTextures(sizeof(m_textures) / sizeof(m_textures[0]), m_textures);
  }

  if (m_depthTexture != 0) {
    glDeleteTextures(1, &m_depthTexture);
  }
}

bool GBuffer::Init(unsigned int width, unsigned int height) {
  CHECK_GL_ERR();
  glGenFramebuffers(1, &m_fbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

  int objSize = sizeof(m_textures) / sizeof(m_textures[0]);

  glGenTextures(objSize, m_textures);
  glGenTextures(1, &m_depthTexture);

  for (unsigned int i = 0 ; i < objSize ; i++) {
    glBindTexture(GL_TEXTURE_2D, m_textures[i]);

    if (i == GBUFFER_TEXTURE_TYPE_SPECULAR_DATA) {
      // specular data requires only 2 16-bit floats
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, NULL);
    } else if (i == GBUFFER_TEXTURE_TYPE_DIFFUSE || i == GBUFFER_TEXTURE_TYPE_SPECULAR_COLOR) {
      // color components are always positive and thus can be placed in GL_R11F_G11F_B10F
      glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    } else {
      // rest of the data has to be in 3 16-bit floats
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
  }

  // depth
  glBindTexture(GL_TEXTURE_2D, m_depthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
               NULL);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

  GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
  glDrawBuffers(sizeof(drawBuffers) / sizeof(drawBuffers[0]), drawBuffers);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if (status != GL_FRAMEBUFFER_COMPLETE) {
    std::string text;
    switch (status) {
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: text = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: text = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
      case GL_FRAMEBUFFER_UNSUPPORTED: text = "GL_FRAMEBUFFER_UNSUPPORTED"; break;
      default: text = "NO DESCRIPTION"; break;
    }
    LOGE("FrameBuffer error, status: [{}] {}", status, text);
    return false;
  }

  // restore default FBO
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  CHECK_GL_ERR();
  return true;
}

void GBuffer::BindForWriting() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void GBuffer::BindForReading() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  int objSize = sizeof(m_textures) / sizeof(m_textures[0]);
  for (unsigned int i = 0; i < objSize; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
  }
  glActiveTexture(GL_TEXTURE0);
}

void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE textureType) {
  glReadBuffer(GL_COLOR_ATTACHMENT0 + textureType);
}

void GBuffer::DebugDraw(float width, float height) {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);

  SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
  glBlitFramebuffer(0, 0, width, height, 0, height * 1.0f / 3.0f, width / 2.0f, height * 2.0f / 3.0f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
  glBlitFramebuffer(0, 0, width, height, 0, height * 2.0f / 3.0f, width / 2.0f, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  glBlitFramebuffer(0, 0, width, height, width / 2.0f, height * 2.0f / 3.0f, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR_DATA);
  glBlitFramebuffer(0, 0, width, height, 0, 0, width / 2.0f, height * 1.0f / 3.0f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR_COLOR);
  glBlitFramebuffer(0, 0, width, height, width / 2.0f, height * 1.0f / 3.0f, width, height * 2.0f / 3.0f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  // this is some place left for the last texture
  //glBlitFramebuffer(0, 0, width, height, width / 2.0f, 0, width, height * 1.0f / 3.0f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}
