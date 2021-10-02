#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 5) in mat4 model;

uniform mat4 lightSpaceMatrix;
void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(vertex_position, 1.0f);
} 


