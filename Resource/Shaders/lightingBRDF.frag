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
in vec2 TexCoords;
in vec3 envMapCoords;

in vec4 FragPosLightSpace;
in vec3 vs_pos;
in vec4 FragPos;
out vec4 colorOutput;


const float PI = 3.14159265359f;
const float prefilterLODLevel = 4.0f;

// Light source(s) informations
struct LightObject
{
    vec3 position;
    vec3 direction;
    vec4 color;
    float radius;
};

uniform int lightPointCounter = 10;
uniform int lightDirectionalCounter = 1;
uniform LightObject lightPointArray[10];
uniform LightObject lightDirectionalArray[1];
uniform int pointLightHide[10];

// G-Buffer
uniform sampler2D gPosition;
uniform sampler2D gAlbedo;
uniform sampler2D gNormal;
uniform sampler2D gEffects;

uniform sampler2D sao;
uniform sampler2D envMap;
uniform samplerCube envMapIrradiance;
uniform samplerCube envMapPrefilter;
uniform sampler2D envMapLUT;

uniform sampler2D shadowMap;
uniform samplerCube shadowCube[10];

uniform sampler2D gBloom;
uniform sampler2D gBloomBlur;

uniform int gBufferView;
uniform bool pointMode;
uniform bool directionalMode;
uniform bool iblMode;
uniform bool envMapShow;
uniform int attenuationMode;
uniform float materialRoughness;
uniform float materialMetallicity;
uniform float ambientIntensity;
uniform vec3 materialF0;
uniform vec4 clearColor;

uniform mat4 view;
uniform mat4 inverseView;
uniform float far_plane;
uniform int dirShadowBool;
uniform int pointShadowBool;
uniform vec3 camPos;
uniform mat4 lightSpaceMatrix;
uniform float ambientAmount;


vec3 colorLinear(vec3 colorVector);
float saturate(float f);
vec2 getSphericalCoord(vec3 normalCoord);
float Fd90(float NoL, float roughness);
float KDisneyTerm(float NoL, float NoV, float roughness);
vec3 computeFresnelSchlick(float NdotV, vec3 F0);
vec3 computeFresnelSchlickRoughness(float NdotV, vec3 F0, float roughness);
float computeDistributionGGX(vec3 N, vec3 H, float roughness);
float computeGeometryAttenuationGGXSmith(float NdotL, float NdotV, float roughness);

float computeShadowDir();
float computeShadow(int ptnum);

void main()
{
    // Retrieve G-Buffer informations
    vec3 viewPos = texture(gPosition, TexCoords).rgb;
    vec3 albedo = colorLinear(texture(gAlbedo, TexCoords).rgb);
    vec3 normal = texture(gNormal, TexCoords).rgb;
    float roughness = texture(gAlbedo, TexCoords).a;
    float metalness = texture(gNormal, TexCoords).a;
    float ao = texture(gEffects, TexCoords).r;
    vec2 velocity = texture(gEffects, TexCoords).gb;
    float depth = texture(gPosition, TexCoords).a;
	
    float sao = texture(sao, TexCoords).r;
    vec3 envColor = texture(envMap, getSphericalCoord(normalize(envMapCoords))).rgb;

    vec3 color = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);
	
    if(depth == 1.0f)
    {
		if(envMapShow)
			color = envColor;
		else
			color = clearColor.rgb;
    }

    else
    {
		roughness = materialRoughness;
        vec3 V = normalize(- viewPos);
        vec3 N = normalize(normal);
        vec3 R = reflect(-V, N);

        float NdotV = max(dot(N, V), 0.0001f);

        // Fresnel (Schlick) computation (F term)
        vec3 F0 = mix(materialF0, albedo, metalness);
        vec3 F = computeFresnelSchlick(NdotV, F0);

        // Energy conservation
        vec3 kS = F;
        vec3 kD = vec3(1.0f) - kS;
        kD *= 1.0f - metalness;

		vec3 ambient = albedo * ambientAmount;
		color += ambient;
        if (pointMode)
        {
            // Point light(s) computation
            for (int i = 0; i < lightPointCounter; i++)
            {
				if(pointLightHide[i] > 0)
					continue;
					
                vec3 L = normalize(lightPointArray[i].position - viewPos);
                vec3 H = normalize(L + V);

                vec3 lightColor = colorLinear(lightPointArray[i].color.rgb);
                float distanceL = length(lightPointArray[i].position - viewPos);
                float attenuation;

                if(attenuationMode == 1)
                    attenuation = 1.0f / (distanceL * distanceL); // Quadratic attenuation
                else if(attenuationMode == 2)
                    attenuation = pow(saturate(1 - pow(distanceL / lightPointArray[i].radius, 4)), 2) / (distanceL * distanceL + 1); // UE4 attenuation

                // Light source dependent BRDF term(s)
                float NdotL = saturate(dot(N, L));

                // Radiance computation
                vec3 kRadiance = lightColor * attenuation;

                // Diffuse component computation
                diffuse = albedo / PI;

                // Disney diffuse term
                float kDisney = KDisneyTerm(NdotL, NdotV, roughness);

                // Distribution (GGX) computation (D term)
                float D = computeDistributionGGX(N, H, roughness);

                // Geometry attenuation (GGX-Smith) computation (G term)
                float G = computeGeometryAttenuationGGXSmith(NdotL, NdotV, roughness);

                // Specular component computation
                specular = (F * D * G) / (4.0f * NdotL * NdotV + 0.0001f);
						
				// Shadow Calulation
				float shadow = 0.0f;
				if(pointShadowBool == 1)
					shadow = computeShadow(i);
				
				shadow *= 1.5;
                //color += ( ((diffuse * kD) * (1.0f - shadow))  + specular * (1.0f - shadow) ) * (kRadiance * (1.0f - shadow)) * NdotL;
				color += ( ((diffuse * kD) * (1.0f - shadow))  + specular ) * (kRadiance) ; // * NdotL
            }
        }

        if (directionalMode)
        {
            for (int i = 0; i < lightDirectionalCounter; i++)
            {
                vec3 L = normalize(- lightDirectionalArray[i].direction);
                vec3 H = normalize(L + V);

                vec3 lightColor = colorLinear(lightDirectionalArray[i].color.rgb);

                // Light source dependent BRDF term(s)
                float NdotL = saturate(dot(N, L));

                // Diffuse component computation
                diffuse = albedo / PI;

                // Disney diffuse term
                float kDisney = KDisneyTerm(NdotL, NdotV, roughness);

                // Distribution (GGX) computation (D term)
                float D = computeDistributionGGX(N, H, roughness);

                // Geometry attenuation (GGX-Smith) computation (G term)
                float G = computeGeometryAttenuationGGXSmith(NdotL, NdotV, roughness);

                // Specular component computation
                specular = (F * D * G) / (4.0f * NdotL * NdotV + 0.0001f);
				
				// Shadow Calulation
				float shadow = 0.0f;
				if(dirShadowBool == 1)
					shadow = computeShadowDir();

				shadow *= 1.95;
				color += ((diffuse * kDisney)+ specular ) * lightColor * NdotL * (1.0f - shadow);
				
				//color += ambient +(( diffuse) * kD + specular ) * lightColor * NdotL * (1.0f - shadow);
                //color += ambient + ((( diffuse * kD) * (1.0f - shadow))  + ((specular * lightColor * NdotL )  * (1.0f - shadow)));
            }
        }

        if (iblMode )
        {
			roughness = materialRoughness;
            F = computeFresnelSchlickRoughness(NdotV, F0, roughness);

            kS = F;
            kD = vec3(1.0f) - kS;
            kD *= 1.0f - metalness;

            // Diffuse irradiance computation
            vec3 diffuseIrradiance = texture(envMapIrradiance, N * mat3(view)).rgb;
            diffuseIrradiance *= albedo;

            // Specular radiance computation
            vec3 specularRadiance = textureLod(envMapPrefilter, R * mat3(view), materialRoughness).rgb;//2.0f * roughness * prefilterLODLevel
            vec2 brdfSampling = texture(envMapLUT, vec2(NdotV, roughness)).rg;
            specularRadiance *= F * brdfSampling.x + brdfSampling.y;
            vec3 ambientIBL = (diffuseIrradiance * kD) + specularRadiance;

            color += ambientIBL;
        }

        color *= ao;
    }


    // Switching between the different buffers
    // Final buffer
    if(gBufferView == 1)
        colorOutput = vec4(color, 1.0f);

    // Position buffer
    else if (gBufferView == 2)
        colorOutput = vec4(viewPos, 1.0f);

    // View Normal buffer
    else if (gBufferView == 3)
        colorOutput = vec4(normal, 1.0f);

    // Color buffer
    else if (gBufferView == 4)
        colorOutput = vec4(albedo, 1.0f);

    // Roughness buffer
    else if (gBufferView == 5)
        colorOutput = vec4(vec3(roughness), 1.0f);

    // Metalness buffer
    else if (gBufferView == 6)
        colorOutput = vec4(vec3(metalness), 1.0f);

    // Depth buffer
    else if (gBufferView == 7)
        colorOutput = vec4(vec3(depth/1000.0f), 1.0f);

    // SAO buffer
    else if (gBufferView == 8)
        colorOutput = vec4(vec3(sao), 1.0f);

    // Velocity buffer
    else if (gBufferView == 9)
        colorOutput = vec4(velocity, 0.0f, 1.0f);

    // Bloom buffer
    else if (gBufferView == 10)
        colorOutput = texture(gBloom, TexCoords);
    //
    // Bloom blur buffer
    else if (gBufferView == 11)
        colorOutput = texture(gBloomBlur, TexCoords);
}



vec3 colorLinear(vec3 colorVector)
{
    vec3 linearColor = pow(colorVector.rgb, vec3(2.2f));

    return linearColor;
}


float saturate(float f)
{
    return clamp(f, 0.0f, 1.0f);
}


vec2 getSphericalCoord(vec3 normalCoord)
{
    float phi = acos(-normalCoord.y);
    float theta = atan(1.0f * normalCoord.x, -normalCoord.z) + PI;

    return vec2(theta / (2.0f * PI), phi / PI);
}


float Fd90(float NoL, float roughness)
{
    return (2.0f * NoL * roughness) + 0.4f;
}


float KDisneyTerm(float NoL, float NoV, float roughness)
{
    return (1.0f + Fd90(NoL, roughness) * pow(1.0f - NoL, 5.0f)) * (1.0f + Fd90(NoV, roughness) * pow(1.0f - NoV, 5.0f));
}


vec3 computeFresnelSchlick(float NdotV, vec3 F0)
{
    return F0 + (1.0f - F0) * pow(1.0f - NdotV, 5.0f);
}


vec3 computeFresnelSchlickRoughness(float NdotV, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(1.0f - NdotV, 5.0f);
}


float computeDistributionGGX(vec3 N, vec3 H, float roughness)
{
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;

    float NdotH = saturate(dot(N, H));
    float NdotH2 = NdotH * NdotH;

    return (alpha2) / (PI * (NdotH2 * (alpha2 - 1.0f) + 1.0f) * (NdotH2 * (alpha2 - 1.0f) + 1.0f));
}


float computeGeometryAttenuationGGXSmith(float NdotL, float NdotV, float roughness)
{
    float NdotL2 = NdotL * NdotL;
    float NdotV2 = NdotV * NdotV;
    float kRough2 = roughness * roughness + 0.0001f;

    float ggxL = (2.0f * NdotL) / (NdotL + sqrt(NdotL2 + kRough2 * (1.0f - NdotL2)));
    float ggxV = (2.0f * NdotV) / (NdotV + sqrt(NdotV2 + kRough2 * (1.0f - NdotV2)));

    return ggxL * ggxV;
}
vec3 gridSamplingDisk[20] = vec3[]
(
	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
	);
float computeShadowDir()
{
	
	vec3 viewPos = texture(gPosition, TexCoords).rgb;
	viewPos = vec3(inverseView * vec4(viewPos, 1.0));
	//viewPos.z = depth;
	
	vec3 position = viewPos + camPos;
	vec4 FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
	
	// perform perspective divide
	vec3 normalizedCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	
	// Transform to [0,1] range
	normalizedCoords = normalizedCoords * 0.5 + 0.5;
	
	// Get closest depth value from light's perspective
	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;
	
	// Get depth of current fragment from light's perspective
	float currentDepth = normalizedCoords.z;
	
	//vec3 normal = normalize(vs_normal);
	vec3 normal = texture(gNormal, TexCoords).rgb;
	vec3 dir = normalize(-lightDirectionalArray[0].direction);
	//dir.y *= -1.f;
	float bias = max(0.05 * (1.0f - dot(normal, dir)), 0.005f);
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, normalizedCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
	
	if (normalizedCoords.z > 1.0f)
		return 0.0f;
		
	return shadow;
	//return 0.0f;
}




// Shadow calulations for point lights
float computeShadow(int ptnum)
{
	//vec3 viewPos = FragPos.xyz;//texture(gPosition, TexCoords).rgb;
	vec3 viewPos = texture(gPosition, TexCoords).rgb;
	viewPos = vec3(inverseView * vec4(viewPos, 1.0));
	//vec3 ftoL = viewPos - vec3(inverseView * lightPointArray[ptnum].position);
	vec3 fragToLight = viewPos - vec3(inverseView * vec4(lightPointArray[ptnum].position,1.0));
	float currentDepth = length(fragToLight);
	float shadow = 0.0f;
	float bias = 0.15f;
	int samples = 20;
	
	float viewDistance = length(camPos - viewPos);
	float diskRadius = (1.0 + (viewDistance / far_plane)) / 50.0;

	for (int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(shadowCube[ptnum], fragToLight + gridSamplingDisk[i] * diskRadius).r;
		closestDepth *= far_plane;   // undo mapping [0;1]
		if (currentDepth - bias > closestDepth)
			shadow += 1.0;
	}
	shadow /= float(samples);

	return shadow;
}