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

  unsigned int objSize = sizeof(m_textures) / sizeof(m_textures[0]);

  glGenTextures(objSize, m_textures);
  glGenTextures(1, &m_depthTexture);

  for (unsigned int i = 0 ; i < objSize ; i++) {
    //TODO create framebuffer textures for every rendering target needed
  }

  //TODO create framebuffer textures for depth buffer (z-buffer)

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
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    return false;
  }

  // restore default FBO
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  CHECK_GL_ERR();
  return true;
}

void GBuffer::BindForWriting() {
  //TODO geometry pass implementation
}

void GBuffer::BindForReading() {
  //TODO bind for reading
}

void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE textureType) {
  glReadBuffer(GL_COLOR_ATTACHMENT0 + textureType);
}

void GBuffer::DebugDraw(GLint width, GLint height) {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);

  SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
  glBlitFramebuffer(0, 0, width, height, 0, height / 3, width / 2, (height * 2) / 3, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
  glBlitFramebuffer(0, 0, width, height, 0, (height * 2) / 3, width / 2, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  glBlitFramebuffer(0, 0, width, height, width / 2, (height * 2) / 3, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR_DATA);
  glBlitFramebuffer(0, 0, width, height, 0, 0, width / 2, height / 3, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR_COLOR);
  glBlitFramebuffer(0, 0, width, height, width / 2, height / 3, width, (height * 2) / 3, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  // this is some place left for the last texture
  //glBlitFramebuffer(0, 0, width, height, width / 2.0f, 0, width, height * 1.0f / 3.0f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}
