/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#pragma once

#include <Common.hpp>

class GBuffer {
 public:

  enum GBUFFER_TEXTURE_TYPE {
    GBUFFER_TEXTURE_TYPE_POSITION,
    GBUFFER_TEXTURE_TYPE_DIFFUSE,
    GBUFFER_TEXTURE_TYPE_NORMAL,
    GBUFFER_TEXTURE_TYPE_SPECULAR_COLOR,
    GBUFFER_TEXTURE_TYPE_SPECULAR_DATA,
    GBUFFER_NUM_TEXTURES
  };

  GBuffer();

  ~GBuffer();

  bool Init(unsigned int width, unsigned int height);

  void BindForWriting();

  void BindForReading();

  void SetReadBuffer(GBUFFER_TEXTURE_TYPE textureType);

  void DebugDraw(float width, float height);

 private:

  GLuint m_fbo;
  GLuint m_textures[GBUFFER_NUM_TEXTURES];
  GLuint m_depthTexture;
};
