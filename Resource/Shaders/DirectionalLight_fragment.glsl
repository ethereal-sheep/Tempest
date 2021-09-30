#version 460
in vec3 vs_color;
in vec3 vs_normal;
in vec3 vs_position;
in vec4 FragPosLightSpace;
in vec2 vs_tex;

out vec4 fs_color;

uniform vec3 LightColor;
uniform vec3 LightDirection;
uniform float LightIntensity;
uniform vec3 CamPosition;

uniform vec3 PointLightPositions[10];
uniform float PointLightIntensity[10];
uniform int PointLightNumber;


uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform samplerCube depthMap;

//uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

vec3 ambient;
vec3 diffuse;
vec3 specular;

float shininess = 32.f;
float ambientStrength = 0.05f;
float specularStrength = 0.5f;


// to make
vec3 pointLightColors = vec3(0.0f, 0.7f, 0.5f);
float pointLightConsts = 0.7f;
float pointLightLinears = 0.15; // or   0.09f
float pointLightQuads = 0.052f; // or  0.032f;


void CalculateDirectionalLight()
{
	//vec3 Normal = normalize(vs_normal);
	//
	//vec4 intensity = vec4(ambient,1.0) * vec4(LightColor, 1.0);
	//vec3 pos = vs_position;
	//
	//vec3 N = normalize(vs_normal);
	//vec3 V = normalize(-pos);
	//vec3 L = normalize(LightDirection);
	//
	////// Blin-Phong
	//vec4 diffuseComp = vec4(diffuse, 1.0) * max(dot(N,L),0)  * vec4(LightColor, 1.0);
	//vec3 halfwayDir = normalize(L+V);
	//vec4 specularComp = vec4(specular,1.0) * pow(max(dot(N, halfwayDir),0), 4 * int(shininess))  * vec4(LightColor, 1.0);
	//float shadow = ShadowCal(FragPosLightSpace);
	//intensity += (diffuseComp + specularComp) * LightIntensity;
	////(1.0 - shadow) *
	//
	//return intensity.xyz;

	//ambient
	ambient += ambientStrength * LightColor;
	
	//diffuse
	vec3 norm = normalize(vs_normal);
	vec3 lightDir_normalized = normalize(-LightDirection); //directional light
	diffuse += max(dot(norm, lightDir_normalized), 0.0f) * LightColor;
	
	//spec
	vec3 viewDir_normalized = normalize(-vs_position.xyz);
	vec3 reflection = reflect(-lightDir_normalized, norm);
	float spec = pow(max(dot(viewDir_normalized, reflection), 0.0f), shininess);
	specular += specularStrength * spec * LightColor;

}

void computePointLight(vec3 lightPosition, vec3 lightColor, float constant, float lin, float quad) 
{
	float distance = length(lightPosition - vs_position.xyz);
	float att = 1.0f / (constant + lin * distance + quad * distance * distance);
	
	//ambient
	ambient += att * ambientStrength * lightColor;
	
	//diffuse
	vec3 norm = normalize(vs_normal);
	vec3 lightDir_normalized = normalize(lightPosition - vs_position.xyz); //point light
	diffuse += att * max(dot(norm, lightDir_normalized), 0.0f) * lightColor;
	
	//spec
	vec3 viewDir_normalized = normalize(viewPos - vs_position.xyz);
	vec3 reflection = reflect(-lightDir_normalized, norm);
	float spec = pow(max(dot(viewDir_normalized, reflection), 0.0f), shininess);
	specular += att * specularStrength * spec * lightColor;
}

float computeShadow()
{
	// perform perspective divide
	vec3 normalizedCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	
	// Transform to [0,1] range
	normalizedCoords = normalizedCoords * 0.5 + 0.5;
	
	// Get closest depth value from light's perspective
	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;
	
	// Get depth of current fragment from light's perspective
	float currentDepth = normalizedCoords.z;
	
	if (normalizedCoords.z > 1.0f)
		return 0.0f;
	
	// Check whether current frag pos is in shadow
	float bias = max(0.05f * (1.0f - dot(vs_normal, LightDirection)), 0.005f);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadow;

	
}

void main()
{
	ambient = vec3(0.0f, 0.0f, 0.0f);
	diffuse = vec3(0.0f, 0.0f, 0.0f);
	specular = vec3(0.0f, 0.0f, 0.0f);
	//
	//vec3 Normal = normalize(vs_normal);

	//vec4 intensity = vec4(ambient,1.0) * vec4(LightColor, 1.0);
	//vec3 pos = vs_position;
	//
	//vec3 N = normalize(vs_normal);
	//vec3 V = normalize(-pos);
	//vec3 L = normalize(LightDirection);
	//
	////// Blin-Phong
	//vec4 diffuseComp = vec4(diffuse, 1.0) * max(dot(N,L),0)  * vec4(LightColor, 1.0);
	//vec3 halfwayDir = normalize(L+V);
	//vec4 specularComp = vec4(specular,1.0) * pow(max(dot(N, halfwayDir),0), 4 * int(shininess))  * vec4(LightColor, 1.0);
	//float shadow = ShadowCal(FragPosLightSpace);
	//intensity +=  (diffuseComp + specularComp) * LightIntensity;
	////(1.0 - shadow) *
	//vec3 posToView = normalize(CamPosition - vs_position);


	//float shadow = 0.f;//ShadowCalculation(vs_position);    
	// POINT LIGHT
	

	//CalculateDirectionalLight();

	//float shadow = ShadowCal(FragPosLightSpace);
	//vec3 posToView = normalize(CamPosition - vs_position);


	for(int i = 0; i< PointLightNumber; i++)
	{
		computePointLight(PointLightPositions[i], pointLightColors, pointLightConsts, pointLightLinears, pointLightQuads);
	}

	//for(int ptLight = 0 ; ptLight < PointLightNumber; ptLight++)
	//{
	//	// Point Lights
	//	vec3 lightDir = normalize(PointLightPositions[ptLight] - vs_position);
	//	float Diffuse = max(dot(Normal, lightDir), 0.0);
	//	vec3 Reflect = normalize(reflect(-lightDir, Normal));
	//	float specConstant = pow(max(dot(posToView, Reflect), 0.0), shininess);
	//	float distance = length(PointLightPositions[ptLight] - vs_position);
	//	float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.0322 * (distance * distance));
	//	vec3 Final_Diffuse = Diffuse * vec3(1.0, 1.0, 1.0);
	//	vec3 Final_Specular = vec3(1.0, 1.0, 1.0) * specConstant;
	//	Final_Diffuse *= attenuation;
	//	Final_Specular *= attenuation;
	//	result += (ambient + (1.0 - shadow) * (Final_Diffuse + Final_Specular)) * PointLightIntensity[ptLight];
	//}
	//fs_color = vec4(vs_color * result, 1.0);


	//	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	// diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	// specular *= texture(specularTexture, fTexCoords).rgb;


	//ambient *= texture(diffuseTexture, vs_tex).rgb;
	//diffuse *= texture(diffuseTexture, vs_tex).rgb;
	//specular *= texture(diffuseTexture, vs_tex).rgb;
	
	ambient *= vs_color;
	diffuse *= vs_color;
	specular *= vs_color;

	float shadow = computeShadow();
	vec3 color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);
	
	fs_color = vec4(color, 1.0f);





	//fs_color = vec4(vs_color, 1.0);
}

