/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#version 330 core
layout(location = 0) in vec3 a_vertexPos;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in vec3 a_normal;

out vec3 v_vertexPos;
out vec2 v_texCoord;
out vec3 v_normal;

uniform mat4 u_mvp;
uniform mat4 u_transform;

void main()
{
  // pass the varying variables to fsh
  v_vertexPos = a_vertexPos;
  v_texCoord = a_texCoord;
  v_normal = a_normal;

  // calculate postion of the vertex
  gl_Position = u_mvp * u_transform * vec4(a_vertexPos, 1);
}
