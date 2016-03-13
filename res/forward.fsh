/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#version 330 core
out vec4 frag_color;

in vec3 v_vertexPos;
in vec2 v_texCoord;
in vec3 v_normal;

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

struct Material
{
	float specularIntensity;
	float specularPower;
};

uniform vec3 u_eyePos;

uniform Material u_material;

// lights
uniform BaseLight u_ambientLight;
uniform DirectionalLight u_directionalLight;

uniform sampler2D gSampler;

vec4 calcLight(BaseLight base, vec3 direction, vec3 normal)
{
	if(base.intensity <= 0.0f)
		return vec4(0.0f,0.0f,0.0f,0.0f);
	
	vec4 diffuseColor = vec4(0.0f,0.0f,0.0f,0.0f);
	vec4 specularColor = vec4(0.0f,0.0f,0.0f,0.0f);
		
	float diffuseFactor =  dot(normal,-direction);	
	
	if(diffuseFactor > 0.0f )
	{
		diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;
		
		vec3 directionToEye = normalize(u_eyePos - v_vertexPos);
		vec3 reflectDirection = normalize(reflect(direction,normal));
		
		float specularFactor = dot(directionToEye,reflectDirection);
		
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, u_material.specularPower);
			specularColor = vec4(base.color, 1.0) * u_material.specularIntensity * specularFactor * base.intensity;
		}
	}
	
	return diffuseColor + specularColor;
}

vec4 calcDirectionalLight(DirectionalLight directionalLight, vec3 normal)
{
	return calcLight(directionalLight.base, -directionalLight.direction, normal);
}

vec4 calcAmbientlLight(BaseLight ambientLight)
{
	if(ambientLight.intensity <= 0.0f)
		return vec4(0.0f,0.0f,0.0f,0.0f);
	
	return vec4(ambientLight.color * ambientLight.intensity, 1);
}

void main()
{
	vec4 color = texture(gSampler, v_texCoord);
		
	vec3 normal = normalize(v_normal);
	vec4 light = calcAmbientlLight(u_ambientLight);
	//light += calcDirectionalLight(u_directionalLight, normal);

    frag_color = color * light;
}
