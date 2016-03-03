/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#pragma once

#include <SDL.h>
#undef main

#define GLEW_STATIC // Link GLEW in static mode
#include <GL/glew.h>
#include <SDL_opengl.h>

#ifdef __APPLE__ // OSX
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#elif __WIN32__ // Windows
#include <gl/gl.h>
#include <gl/glu.h>
#else  // Linux
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Logger.hpp>

#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <memory>

typedef int32_t int_t;
typedef int64_t long_t;
typedef uint32_t uint_t;
typedef uint64_t ulong_t;
