/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#version 330 core

/* Structures */

struct Material {
	float specularIntensity;
	float specularPower;
	vec3 specularColor;
};

/* Params */

out vec4 frag_color;

in vec3 v_vertexPos;
in vec2 v_texCoord;
in vec3 v_normal;

layout (location = 0) out vec3 vertexPosOut;
layout (location = 1) out vec3 diffuseOut;
layout (location = 2) out vec3 normalOut;
layout (location = 3) out vec3 specularColorOut;
layout (location = 4) out vec2 specularDataOut;

uniform Material u_material;
uniform sampler2D gSampler;

/* Functions */

void main()
{
	// pass data to different rendering targets
	vertexPosOut = v_vertexPos;
	diffuseOut = texture(gSampler, v_texCoord).xyz;	
	normalOut = v_normal;
	specularColorOut = u_material.specularColor;
	specularDataOut = vec2(u_material.specularIntensity, u_material.specularPower);
}
