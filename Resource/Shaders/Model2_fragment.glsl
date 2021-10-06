#version 460
// Model2_fragment
in vec3 vs_color;
in vec3 vs_normal;
in vec3 vs_position;
in vec4 FragPosLightSpace;
in vec2 vs_tex;

out vec4 fs_color;

// Directional Light Variables
uniform vec3  LightColor;
uniform vec3  LightDirection;
uniform float LightIntensity;
uniform vec3  CamPosition;
uniform int   DirectionalLightOn;

// Point Light Variables
uniform vec3  PointLightPositions[10];
uniform float PointLightIntensity[10];
uniform int   PointLightNumber;
uniform vec3  pointLightColors[10];
uniform float pointLightConsts[10];
uniform float pointLightLinears[10];
uniform float pointLightQuads[10];
uniform int pointLightHide[10];


uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;  // In slot 5
uniform samplerCube depthMap; // In slot 6

uniform vec3  viewPos;
uniform float far_plane;
uniform int   GammaCorrection;

// Object Values
uniform float shininess;
uniform float ambientStrength;
uniform float specularStrength;

uniform vec3 DiffuseColour;

// Declaration of variables
vec3 ambient;
vec3 diffuse;
vec3 specular;

vec3 gridSamplingDisk[20] = vec3[]
(
	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
	);

// Compute the shadows for directional lights
float computeShadowDir()
{
	// perform perspective divide
	vec3 normalizedCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;

	// Transform to [0,1] range
	normalizedCoords = normalizedCoords * 0.5 + 0.5;

	// Get closest depth value from light's perspective
	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;

	// Get depth of current fragment from light's perspective
	float currentDepth = normalizedCoords.z;

	vec3 normal = normalize(vs_normal);
	float bias = max(0.05 * (1.0f - dot(normal, LightDirection)), 0.005f);
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
}

// Calculate the Light for Directional Lights
void CalculateDirectionalLight()
{
	vec3 pos = vs_position;
	vec3 N = normalize(vs_normal);
	vec3 V = normalize(CamPosition-pos);
	vec3 L = normalize(LightDirection);

	float dirShadow = computeShadowDir();

	////// Blin-Phong

	//ambient
	ambient += ambientStrength * LightColor;

	//diffuse
	diffuse += (1.0f - dirShadow) * ((max(dot(N, L), 0) * vec4(LightColor, 1.0)).xyz);

	//spec
	vec3 halfwayDir = normalize(L + V);
	specular += (1.0f - dirShadow) * ((pow(max(dot(N, halfwayDir), 0), 4 * int(shininess)) * vec4(LightColor, 1.0)).xyz);
}

// Calculate the Light for Point Lights
void computePointLight(vec3 lightPosition, vec3 lightColor, float constant, float lin, float quad, float shadow)
{
	float distance = length(lightPosition - vs_position.xyz);
	float att = 1.0f / (constant + lin * distance + quad * distance * distance);

	//ambient
	ambient += att * ambientStrength * lightColor;

	//diffuse
	vec3 norm = normalize(vs_normal);
	vec3 lightDir_normalized = normalize(lightPosition - vs_position.xyz); //point light
	diffuse += (1.0f - shadow) * (att * max(dot(norm, lightDir_normalized), 0.0f) * lightColor);

	//spec
	vec3 viewDir_normalized = normalize(viewPos - vs_position.xyz);
	vec3 reflection = reflect(-lightDir_normalized, norm);
	float spec = pow(max(dot(viewDir_normalized, reflection), 0.0f), shininess);
	specular += (1.0f - shadow) * (att * specularStrength * spec * lightColor);
}


// Shadow calulations for point lights
float computeShadow(int ptnum)
{
	vec3 fragToLight = vs_position - PointLightPositions[ptnum];
	float currentDepth = length(fragToLight);
	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(viewPos - vs_position);
	float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;

	for (int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
		closestDepth *= far_plane;   // undo mapping [0;1]
		if (currentDepth - bias > closestDepth)
			shadow += 1.0;
	}
	shadow /= float(samples);

	return shadow;
}


void main()
{
	ambient = vec3(0.0f, 0.0f, 0.0f);
	diffuse = vec3(0.0f, 0.0f, 0.0f);
	specular = vec3(0.0f, 0.0f, 0.0f);

	if (DirectionalLightOn > 0)
		CalculateDirectionalLight();    // Directional light

	// POINT LIGHT
	for (int i = 0; i < PointLightNumber; i++)
	{
		if(pointLightHide[i] > 0)
			continue;
		float shadow = computeShadow(i);
		computePointLight(PointLightPositions[i], pointLightColors[i], pointLightConsts[i], pointLightLinears[i], pointLightQuads[i], shadow);
	}

	// For Texture 
	//ambient *= texture(diffuseTexture, vs_tex).rgb;
	//diffuse *= texture(diffuseTexture, vs_tex).rgb;
	//specular *= texture(diffuseTexture, vs_tex).rgb;

	ambient *= DiffuseColour;
	diffuse *= DiffuseColour;
	specular *= DiffuseColour;


	vec3 color = min((ambient + diffuse) + specular, 1.0f);

	// gamma correction
	if (GammaCorrection > 0)
		color = pow(color, vec3(1.0 / 2.2));

	fs_color = vec4(color * vec3(texture(diffuseTexture, vs_tex)), 1.0f);
}