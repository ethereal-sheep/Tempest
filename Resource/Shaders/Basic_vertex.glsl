#version 460

layout(location = 0)  in vec3 vertex_position;
layout(location = 1)  in vec2 texCoord;
layout(location = 2)  in vec3 normal;
layout(location = 3)  in vec3 tangent;
layout(location = 4)  in vec3 bitangent;

out vec3 vs_color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	vs_color = vec3(texCoord, 0.f);
	gl_Position =  ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);
}