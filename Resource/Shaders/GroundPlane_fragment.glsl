#version 460

//layout(origin_upper_left) in vec4 gl_FragCoord;

uniform vec3 camera;
uniform vec3 front;
uniform mat4 view;
uniform mat4 proj;


layout(location = 1) in vec3 nearPoint; // nearPoint calculated in vertex shader
layout(location = 2) in vec3 farPoint; // farPoint calculated in vertex shader
layout(location = 0) out vec4 outColor;

void main() 
{
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    outColor = vec4(1.0, 0.0, 0.0, 1.0 * float(t > 0)); // opacity = 1 when t > 0, opacity = 0 otherwise
}