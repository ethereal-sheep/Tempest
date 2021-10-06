#version 460

layout(location = 0)  in vec3 vertex_position;
layout(location = 1)  in vec2 texCoord;
layout(location = 2)  in vec3 normal;
layout(location = 3)  in vec3 tangent;
layout(location = 4)  in vec3 bitangent;
layout(location = 5)  in mat4 model;

in vec4 fs_color;
out vec3 vs_color;
out vec3 vs_normal;
out vec3 vs_position;
out vec4 FragPosLightSpace;
out vec2 vs_tex;
//uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform int PointLightNumber;
uniform mat4 lightSpaceMatrix;

void main()
{
	vs_position = (model * vec4(vertex_position, 1.0)).xyz;
	vs_normal = (transpose(inverse(model)) * vec4(normal, 1.0)).xyz; 
	vs_color = normal;
	vs_tex = texCoord;
	FragPosLightSpace = lightSpaceMatrix * vec4(vs_position, 1.0);
	gl_Position =  ProjectionMatrix * ViewMatrix * model * vec4(vertex_position, 1.f);
}