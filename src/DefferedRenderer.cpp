/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#include <Renderer.hpp>

DeferredRenderer::DeferredRenderer(std::shared_ptr<Scene> scene)
  : Renderer(scene) {
  LOGD("Created deferred renderer");

  type = RendererType::DEFERRED;
}

DeferredRenderer::~DeferredRenderer() {

}

void DeferredRenderer::InitRenderer() {

  for (auto &mesh : m_scene->m_meshes)
  { mesh->SetShader(m_shader); }

}

void DeferredRenderer::RenderScene(float) {

}
