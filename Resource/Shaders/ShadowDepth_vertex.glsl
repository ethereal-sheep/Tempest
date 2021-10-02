#version 460

layout(location = 0) in vec3 vertex_position;

layout(location = 5)  in mat4 model;
uniform mat4 lightSpaceMatrix;
//uniform mat4 model;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(vertex_position, 1.0);
   // gl_Position =  ProjectionMatrix * ViewMatrix * model * vec4(vertex_position, 1.f);
}

