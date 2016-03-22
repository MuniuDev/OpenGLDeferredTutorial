/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#version 330 core

/* Structures */

struct BaseLight {
	vec3 color;
	float intensity;
};

struct DirectionalLight {
	BaseLight base;
	vec3 direction;
};

struct PointLight {
	BaseLight base;
	vec3 position;
	float range;
	float attenuation;
};

struct Material {
	float specularIntensity;
	float specularPower;
	vec3 specularColor;
};

/* Params */

out vec4 frag_color;

uniform vec3 u_eyePos;
uniform vec2 u_screenSize;

// lights
uniform BaseLight u_ambientLight;
uniform DirectionalLight u_directionalLight;
uniform PointLight u_pointLights[10];
uniform int u_lightCount;

uniform sampler2D gPositionMap;
uniform sampler2D gColorMap;
uniform sampler2D gNormalMap;
uniform sampler2D gSpecularColorMap;
uniform sampler2D gSpecularDataMap;

/* Functions */

// calculates ambient light
vec4 ambientlLight(BaseLight ambientLight) {
	if(ambientLight.intensity <= 0.0f) {
		return vec4(0.0f,0.0f,0.0f,0.0f);
	}
	
	return vec4(ambientLight.color * ambientLight.intensity, 1);
}

// calculates diffuse factor of any light
vec4 diffuseLight(BaseLight base, vec3 direction, vec3 normal) {
	if(base.intensity <= 0.0f)
		return vec4(0.0f,0.0f,0.0f,0.0f);
	
	vec4 diffuseColor = vec4(0.0f,0.0f,0.0f,0.0f);	
	float diffuseFactor =  dot(normal,-direction);	
	
	if(diffuseFactor > 0.0f ) {
		diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;
	}
	
	return diffuseColor;
}

// calculates specular factor of any light
vec4 specularLight(BaseLight base, vec3 direction, vec3 normal, vec3 vertexPos, Material mat) {
	if(base.intensity <= 0.0f) {
		return vec4(0.0f,0.0f,0.0f,0.0f);
	}
	
	vec4 specularColor = vec4(0.0f,0.0f,0.0f,0.0f);	
	vec3 directionToEye = normalize(u_eyePos - vertexPos);
	vec3 reflectDirection = normalize(reflect(direction,normal));
	
	float specularFactor = dot(directionToEye,reflectDirection);
	
	if(specularFactor > 0.0f) {
		specularFactor = pow(specularFactor, mat.specularPower);
		specularColor = vec4(base.color, 1.0) * mat.specularIntensity * specularFactor * base.intensity;
	}
	
	return specularColor * vec4(mat.specularColor,1);
}

// calculates attenuation of the point light
float pointLightAttenuation(PointLight light, vec3 vertexPos) {
	float distanceToLight = length(light.position - vertexPos);
	if( distanceToLight > light.range ) {
		return 0.0f;
	}
    return 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));
}

void main()
{
   	// get data from textures
	vec2 texCoord = gl_FragCoord.xy / u_screenSize;
    vec3 worldPos = texture(gPositionMap, texCoord).xyz;
   	vec4 tex_color = vec4(texture(gColorMap, texCoord).xyz, 0);
   	vec3 normal = normalize(texture(gNormalMap, texCoord).xyz);

	Material mat;
	mat.specularColor = texture(gSpecularColorMap, texCoord).xyz;
	vec2 data = texture(gSpecularDataMap, texCoord).xy;
	mat.specularIntensity = data.x;
	mat.specularPower = data.y;

	// ambient
	vec4 ambient = tex_color * ambientlLight(u_ambientLight);

	// directional (diffuse + specular)
	vec4 directional = tex_color * diffuseLight(u_directionalLight.base, u_directionalLight.direction, normal);
	directional += specularLight(u_directionalLight.base, u_directionalLight.direction, normal, worldPos, mat);

	// point lights
	vec4 point = vec4(0,0,0,0);
	for(int i=0; i<u_lightCount; ++i) {
		vec4 tmp = tex_color * diffuseLight(u_pointLights[i].base, normalize(u_pointLights[i].position - worldPos), normal);
		tmp += specularLight(u_pointLights[i].base, normalize(u_pointLights[i].position - worldPos), normal, worldPos, mat);
		point += tmp * pointLightAttenuation(u_pointLights[i], worldPos);
	}

	// combine lights for final color
    frag_color = ambient + directional + point;
}
