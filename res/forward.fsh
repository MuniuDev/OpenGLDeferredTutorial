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
	vec3 specularColor;
};

uniform vec3 u_eyePos;

uniform Material u_material;

// lights
uniform BaseLight u_ambientLight;
uniform DirectionalLight u_directionalLight;

uniform sampler2D gSampler;
uniform mat4 u_transform;

vec4 ambientlLight(BaseLight ambientLight)
{
	if(ambientLight.intensity <= 0.0f)
		return vec4(0.0f,0.0f,0.0f,0.0f);
	
	return vec4(ambientLight.color * ambientLight.intensity, 1);
}

vec4 diffuseLight(BaseLight base, vec3 direction, vec3 normal) {
	if(base.intensity <= 0.0f)
		return vec4(0.0f,0.0f,0.0f,0.0f);
	
	vec4 diffuseColor = vec4(0.0f,0.0f,0.0f,0.0f);	
	float diffuseFactor =  dot(normal,-direction);	
	
	if(diffuseFactor > 0.0f )
	{
		diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;
	}
	
	return diffuseColor;
}

vec4 specularLight(BaseLight base, vec3 direction, vec3 normal, vec3 vertexPos) {
	if(base.intensity <= 0.0f)
		return vec4(0.0f,0.0f,0.0f,0.0f);
	
	vec4 specularColor = vec4(0.0f,0.0f,0.0f,0.0f);
		
	vec3 directionToEye = normalize(u_eyePos - vertexPos);
	vec3 reflectDirection = normalize(reflect(direction,normal));
	
	float specularFactor = dot(directionToEye,reflectDirection);
	
	if(specularFactor > 0.0f)
	{
		specularFactor = pow(specularFactor, u_material.specularPower);
		specularColor = vec4(base.color, 1.0) * u_material.specularIntensity * specularFactor * base.intensity;
	}
	
	return specularColor * vec4(u_material.specularColor,1);
}

void main()
{
	vec4 tex_color = texture(gSampler, v_texCoord);
	
	// transform position and normal to world coordinates
	vec3 normal = normalize(transpose(inverse(mat3(u_transform))) * v_normal);
	vec3 vertexPos = vec3(u_transform * vec4(v_vertexPos, 1));
	
	// ambient
	vec4 ambient = tex_color * ambientlLight(u_ambientLight);
	
	// directional (diffuse + specular)
	vec4 directional = tex_color * diffuseLight(u_directionalLight.base, u_directionalLight.direction, normal);
	directional += specularLight(u_directionalLight.base, u_directionalLight.direction, normal, vertexPos);
	
	// calculate final color
    frag_color = ambient + directional;
}
