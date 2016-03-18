/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#version 330 core

in vec3 v_vertexPos;
in vec2 v_texCoord;
in vec3 v_normal;

layout (location = 0) out vec3 vertexPosOut;
layout (location = 1) out vec3 diffuseOut;
layout (location = 2) out vec3 normalOut;
layout (location = 3) out vec3 texCoordOut;

out vec4 frag_color;

uniform sampler2D gSampler;


void main()
{
	vertexPosOut = v_vertexPos;
	diffuseOut = texture(gSampler, v_texCoord).xyz;	
	normalOut = normalize(v_normal);
	texCoordOut = vec3(v_texCoord, 0.0);
}
