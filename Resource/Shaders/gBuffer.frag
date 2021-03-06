#version 460 core
/**********************************************************************************
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \author		Tiong Jun Ming, Jerome (j.tiong@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gAlbedo;
layout (location = 2) out vec4 gNormal;
layout (location = 3) out vec3 gEffects;
layout (location = 5) out vec4 gViewPos;
layout (location = 4) out vec4 gBloom;
in vec3 viewPos;
in vec2 TexCoords;
in vec3 normal;
in vec4 fragPosition;
in vec4 fragPrevPosition;

const float nearPlane = 1.0f;
const float farPlane = 1000.0f;

uniform vec3 albedoColor;
uniform sampler2D texAlbedo;
uniform sampler2D texNormal;
uniform sampler2D texRoughness;
uniform sampler2D texMetalness;
uniform sampler2D texAO;
uniform vec3 colour;
uniform int texID;
uniform int TestPBR;
uniform bool emissive;
float LinearizeDepth(float depth);
vec3 computeTexNormal(vec3 viewNormal, vec3 texNormal);


void main()
{
    vec4 bloomColor = vec4(0.0, 0.0, 0.0, 1.0);
	vec3 texNormal = normalize(texture(texNormal, TexCoords).rgb * 2.0f - 1.0f);
    texNormal.g = -texNormal.g;   // In case the normal map was made with DX3D coordinates system in mind

    vec2 fragPosA = (fragPosition.xy / fragPosition.w) * 0.5f + 0.5f;
    vec2 fragPosB = (fragPrevPosition.xy / fragPrevPosition.w) * 0.5f + 0.5f;

    gPosition = vec4(viewPos, LinearizeDepth(gl_FragCoord.z));
    //gAlbedo.rgb = vec3(texture(texAlbedo, TexCoords));
//    gAlbedo.rgb = vec3(albedoColor);
	if (texID != 0)
	{
		gAlbedo.rgb = vec3(texture(texAlbedo, TexCoords));
		//gAlbedo.rgb *= 1.2;
		bloomColor.rgb = vec3(texture(texAlbedo, TexCoords));
				
	}
	else
	{
		gAlbedo.rgb = vec3(colour);
		//gAlbedo.rgb *= 1.5;
		if (gAlbedo.r > 0.05f  && gAlbedo.r < 0.5f)
			gAlbedo.r *= 1.2f;
		if (gAlbedo.g > 0.05f  && gAlbedo.g < 0.5f)
			gAlbedo.g *= 1.2f;
		if (gAlbedo.b > 0.05f  && gAlbedo.b < 0.5f)
			gAlbedo.b *= 1.2f;

		bloomColor.rgb = vec3(colour);
	}
	//gAlbedo.rgb = vec3(colour);
	//if(TestPBR == 1)
	//{
	//	gAlbedo.rgb = vec3(texture(texAlbedo, TexCoords));
	//}
	//else
	//{
	//	gAlbedo.rgb = vec3(colour);
	//}
    gAlbedo.a =  vec3(texture(texRoughness, TexCoords)).r;
	gAlbedo.a =  0.0f;
	
    //gNormal.rgb = computeTexNormal(normal, texNormal);
    gNormal.rgb = normalize(normal);
	
    gNormal.a = vec3(texture(texMetalness, TexCoords)).r;
    gEffects.r = vec3(texture(texAO, TexCoords)).r;
    gEffects.gb = fragPosA - fragPosB;
	gViewPos = fragPosition;

	if (emissive)
		gBloom = bloomColor;
	else
		gBloom = vec4(0.0, 0.0, 0.0, 1.0);
}



float LinearizeDepth(float depth)
{
    float z = depth * 2.0f - 1.0f;
    return (2.0f * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}


vec3 computeTexNormal(vec3 viewNormal, vec3 texNormal)
{
    vec3 dPosX  = dFdx(viewPos);
    vec3 dPosY  = dFdy(viewPos);
    vec2 dTexX = dFdx(TexCoords);
    vec2 dTexY = dFdy(TexCoords);

    vec3 normal = normalize(viewNormal);
    vec3 tangent = normalize(dPosX * dTexY.t - dPosY * dTexX.t);
    vec3 binormal = -normalize(cross(normal, tangent));
    mat3 TBN = mat3(tangent, binormal, normal);

    return normalize(TBN * texNormal);
}
