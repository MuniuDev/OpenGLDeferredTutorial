/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#include <Viewport.hpp>

#include <Input.hpp>
//#include <Mesh.hpp>

Viewport::Viewport(float width, float height)
  : m_width(width),
    m_height(height),
    m_camera(nullptr) {

}

void Viewport::Resize(float width, float height) {
  LOGD("Resizing viewport to {}x{}", width, height);
  m_width = width;
  m_height = height;
  m_camera->Resize(45.0f, m_width / m_height, 0.1f, 1000.0f);

  m_renderer->Resize(m_width, m_height);
}

void Viewport::Init() {
  m_camera = std::make_shared<Camera>(45.0f, m_width / m_height, 0.1f, 1000.0f);

  InitSmallScene();
  InitBigScene();

  m_scene = m_smallScene;

  m_deferredRenderer = std::make_shared<DeferredRenderer>();
  m_forwardRenderer = std::make_shared<ForwardRenderer>();

  m_renderer = m_forwardRenderer;
  m_renderer->InitRenderer(m_scene, m_width, m_height);
  CHECK_GL_ERR();
}

void Viewport::InitBigScene() {

  m_bigScene = std::make_shared<Scene>();
  m_bigScene->Init(m_camera);

  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 4; j++) {
      auto tank = MeshFactory::GetInstance().LoadMesh(GetPath("res/model-tank/"), "tank.fbx");

      pos.x = (float)i * 6 - 15;
      pos.z = (float)j * 10 - 15;
      tank->SetPos(pos);
      m_bigScene->AddMesh(tank);
    }
  }

  PointLight light = PointLight(glm::vec3(0, 0, 0), 1, glm::vec3(0, 0, 0), 100, 1);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {

      light.base = BaseLight(glm::vec3(GetRandom(), GetRandom(), GetRandom()),
                             50 + 30 * GetRandom());

      light.range = 50.0f + 100.0f * GetRandom();

      light.position.x = i * 14 + 3 * GetRandom() - 15;
      light.position.y = 3.0f * (GetRandom() + 0.2f);
      light.position.z = j * 12 + 3 * GetRandom() - 15;

      m_bigScene->AddPointLight(light);
    }
  }
}

void Viewport::InitSmallScene() {
  m_smallScene = std::make_shared<Scene>();
  m_smallScene->Init(m_camera);

  std::shared_ptr<MeshNode> tank;
  tank = MeshFactory::GetInstance().LoadMesh(GetPath("res/model-tank/"), "tank.fbx");

  m_smallScene->AddMesh(tank);

  m_smallScene->AddPointLight(PointLight(glm::vec3(1, 0, 0), 3, glm::vec3(-1, 2, 0), 100, 1));
  m_smallScene->AddPointLight(PointLight(glm::vec3(0, 1, 0), 3, glm::vec3(1, 2, 0), 100, 1));
}

void Viewport::Draw(float dt) {

  m_scene->Update(dt);

  if (g_input.GetKeyState(SDL_SCANCODE_F))
    SetActiveRenderer(RendererType::FORWARD);
  else if (g_input.GetKeyState(SDL_SCANCODE_R))
    SetActiveRenderer(RendererType::DEFERRED);


  static unsigned int timer = SDL_GetTicks();

  if (timer < SDL_GetTicks()) {
    if (g_input.GetKeyState(SDL_SCANCODE_N)) {
      m_scene = m_smallScene;
      m_scene->Init(m_camera);
      m_renderer->InitRenderer(m_scene, m_width, m_height);
      LOGD("Changed scene to smallScene");
      timer = SDL_GetTicks() + 500;
    } else if (g_input.GetKeyState(SDL_SCANCODE_M)) {
      m_scene = m_bigScene;
      m_scene->Init(m_camera);
      m_renderer->InitRenderer(m_scene, m_width, m_height);
      LOGD("Changed scene to bigScene");
      timer = SDL_GetTicks() + 500;
    }
  }

  glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);

  m_renderer->RenderScene(dt);
}

void Viewport::SetActiveRenderer(RendererType type) {

  if (type != m_renderer->GetType()) {
    switch (type) {
      case RendererType::FORWARD:
        m_renderer = m_forwardRenderer;
        LOGD("Changed renderer to forward");
        break;
      case RendererType::DEFERRED:
        m_renderer = m_deferredRenderer;
        LOGD("Changed renderer to deferred");
        break;
      default:
        break;
    }

    m_renderer->InitRenderer(m_scene, m_width, m_height);
  }
}
