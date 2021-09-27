#version 460

layout(location = 0)  in vec3 vertex_position;
layout(location = 1)  in vec2 texCoord;
layout(location = 2)  in vec3 normal;
layout(location = 3)  in vec3 tangent;
layout(location = 4)  in vec3 bitangent;
layout(location = 5)  in mat4 model;
layout(location = 9)  in mat3 normalMatrix;

out vec3 vs_color;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	vs_color = normal;
	gl_Position =  ProjectionMatrix * ViewMatrix * model * vec4(vertex_position, 1.f);
}