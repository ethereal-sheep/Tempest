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
uniform float PointLightIntensity[10];
uniform int PointLightNumber;


uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

//uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

float ShadowCalculation(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - PointLightPositions[0];
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}  

void main()
{
	vec3 ambient = vec3(0.3, 0.3, 0.3);
	vec3 diffuse = vec3(0.8, 0.8, 0.8);
	vec3 specular = vec3(.5, .5, .5);
	float shininess = 32.f;
	vec3 Normal = normalize(vs_normal);

	vec4 intensity = vec4(ambient,1.0) * vec4(LightColor, 1.0);
	vec3 pos = vs_position;

	vec3 N = normalize(vs_normal);
	vec3 V = normalize(-pos);
	vec3 L = normalize(LightDirection);

	//// Blin-Phong
	vec4 diffuseComp = vec4(diffuse, 1.0) * max(dot(N,L),0)  * vec4(LightColor, 1.0);
	vec3 halfwayDir = normalize(L+V);
	vec4 specularComp = vec4(specular,1.0) * pow(max(dot(N, halfwayDir),0), 4 * int(shininess))  * vec4(LightColor, 1.0);
	intensity += (diffuseComp + specularComp) * LightIntensity;
	
	vec3 posToView = normalize(CamPosition - vs_position);


	float shadow = 0.f;//ShadowCalculation(vs_position);    
	// POINT LIGHT
	vec3 result = intensity.xyz;

	for(int ptLight = 0 ; ptLight < PointLightNumber; ptLight++)
	{
		// Point Lights
		vec3 lightDir = normalize(PointLightPositions[ptLight] - vs_position);
		float Diffuse = max(dot(Normal, lightDir), 0.0);
		vec3 Reflect = normalize(reflect(-lightDir, Normal));
		float specConstant = pow(max(dot(posToView, Reflect), 0.0), shininess);
		float distance = length(PointLightPositions[ptLight] - vs_position);
		float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.0322 * (distance * distance));
		vec3 Final_Diffuse = Diffuse * vec3(1.0, 1.0, 1.0);
		vec3 Final_Specular = vec3(1.0, 1.0, 1.0) * specConstant;
		Final_Diffuse *= attenuation;
		Final_Specular *= attenuation;
		result += (ambient + (1.0 - shadow) * (Final_Diffuse + Final_Specular)) * PointLightIntensity[ptLight];
	}
	fs_color = vec4(vs_color * result, 1.0);
}

