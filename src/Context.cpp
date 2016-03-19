/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#include <Context.hpp>


Context::Context(float width, float height, std::string name)
  : m_viewport(width, height),
    m_width(width),
    m_height(height),
    m_window(nullptr),
    m_context(nullptr) {
  //Initialize SDL
  if (SDL_Init( SDL_INIT_VIDEO) < 0) {
    LOGE("SDL could not initialize! SDL Error: {}\n", SDL_GetError());
    std::exit(-1);
  }

  //Use OpenGL 3.3 core
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  //Create window
  m_window = SDL_CreateWindow(name.c_str(),
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              m_width, m_height,
                              SDL_WINDOW_OPENGL |
                              SDL_WINDOW_SHOWN |
                              SDL_WINDOW_RESIZABLE);
  if (m_window == nullptr) {
    LOGE("Window could not be created! SDL Error: {}", SDL_GetError());
    std::exit(-1);
  }

  //Create context
  m_context = SDL_GL_CreateContext( m_window );
  if (m_context == nullptr) {
    LOGE("OpenGL context could not be created! SDL Error: {}", SDL_GetError());
    std::exit(-1);
  }
  CHECK_GL_ERR();

  //Initialize GLEW
  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if (glewError != GLEW_OK) {
    LOGW("Failed to initialize GLEW! {}", glewGetErrorString(glewError));
  }
  CHECK_GL_ERR();

  //Use Vsync
  if (SDL_GL_SetSwapInterval( 0 ) < 0) {
    LOGW("Unable to set VSync! SDL Error: {}", SDL_GetError());
  }
  CHECK_GL_ERR();

  LOGD("Success initializing graphics context!");
}

Context::~Context() {
  //Destroy window
  if (m_window) {
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
  }
  //Quit SDL subsystems
  SDL_Quit();
}

void Context::Tic() {
  Draw();
  SwapBuffers();
}

void Context::InitGL() {
  // Basic openGL settings
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  CHECK_GL_ERR();

  // init viewports
  m_viewport.Init();
  m_fpsCounter.Init();
}

void Context::HandleWindowEvent(const SDL_Event &event) {
  switch (event.window.event) {
    case SDL_WINDOWEVENT_RESIZED:
      m_width = event.window.data1;
      m_height = event.window.data2;
      m_viewport.Resize(m_width, m_height);
      break;
    default:
      break;
  }
}

void Context::Draw() {
  //drawing here
  float dt = m_fpsCounter.TicAndGetDeltaTime();
  m_viewport.Draw(dt);
  m_fpsCounter.Print();
}

void Context::SwapBuffers() {
  SDL_GL_SwapWindow(m_window);
}
