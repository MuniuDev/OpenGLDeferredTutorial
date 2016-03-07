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

out vec2 texCoord;

uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp * vec4(a_vertexPos,1);
	texCoord = a_texCoord;
}
