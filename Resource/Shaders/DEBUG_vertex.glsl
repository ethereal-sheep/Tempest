#version 460

layout(location = 0)  in vec3 vertex_position;
layout(location = 1)  in vec2 texCoord;
layout(location = 5)  in mat4 model;
out vec2 TexCoords;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;


void main()
{
    TexCoords = texCoord;
    gl_Position = vec4(vertex_position, 1.0);
    //gl_Position =  ProjectionMatrix * ViewMatrix * model * vec4(vertex_position, 1.f);

}