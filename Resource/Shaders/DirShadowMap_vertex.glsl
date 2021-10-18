#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 5) in mat4 model;

uniform mat4 lightSpaceMatrix;

uniform int meshDrawing;
uniform mat4 ModelMatrix;

void main()
{
    if(meshDrawing < 1)
        gl_Position = lightSpaceMatrix * model * vec4(vertex_position, 1.0f);
    else
        gl_Position = lightSpaceMatrix * ModelMatrix * vec4(vertex_position, 1.0f);
} 


