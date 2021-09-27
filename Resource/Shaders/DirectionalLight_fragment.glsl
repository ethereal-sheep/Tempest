#version 460

in vec3 vs_color;
in vec3 vs_normal;
in vec3 vs_position;

out vec4 fs_color;

uniform vec3 LightColor;
uniform vec3 LightDirection;
uniform float LightIntensity;
uniform vec3 CamPosition;

uniform vec3 PointLightPositions[10];
uniform float PointLightIntensity;

void main()
{
	vec3 ambient = vec3(0.1, 0.1, 0.1);
	vec3 diffuse = vec3(0.8, 0.8, 0.8);
	vec3 specular = vec3(1.0, 1.0, 1.0);
	float shininess = 32.f;
	vec3 Normal = normalize(vs_normal);

	float Diffuse = max(dot(Normal, LightDirection), 0.0);
	vec3 Final_Diffuse = Diffuse * diffuse;

	vec3 Reflect = normalize(reflect(-LightDirection, Normal));
	vec3 posToView = normalize(CamPosition - vs_position);
	float specConstant = pow(max(dot(posToView, Reflect), 0.0), shininess);
	vec3 Final_Specular = vec3(1.0, 1.0, 1.0) * specConstant * LightColor;

	float NL = clamp(dot(Normal, LightDirection), 0.0, 1.0);
	
	float shadowFactor = 1.f;
	float shadowCoef = NL * shadowFactor;
	
	vec3 result = (ambient + (Final_Diffuse + Final_Specular) * shadowCoef) * LightIntensity;

	// Point Lights
	vec3 lightDir = normalize(PointLightPositions[0] - vs_position);
	Diffuse = max(dot(Normal, lightDir), 0.0);
	Reflect = normalize(reflect(-lightDir, Normal));
	specConstant = pow(max(dot(posToView, Reflect), 0.0), shininess);
	float distance = length(PointLightPositions[0] - vs_position);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.0322 * (distance * distance));
	Final_Diffuse = Diffuse * vec3(1.0, 1.0, 1.0);
	Final_Specular = vec3(1.0, 1.0, 1.0) * specConstant;
	Final_Diffuse *= attenuation;
	Final_Specular *= attenuation;
	result += (ambient + Final_Diffuse + Final_Specular) * PointLightIntensity;

	fs_color = vec4(vs_color * result, 1.0);

	//_color = vec4(vs_color, 1.0);
}