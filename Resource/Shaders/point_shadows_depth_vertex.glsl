#version 460
layout (location = 0) in vec3 aPos;

layout(location = 5)  in mat4 model;
 
uniform int meshDrawing;
uniform mat4 ModelMatrix;

void main()
{
    if(meshDrawing < 1)
        gl_Position = model * vec4(aPos, 1.0);
    else
        gl_Position = ModelMatrix * vec4(aPos, 1.0);
}