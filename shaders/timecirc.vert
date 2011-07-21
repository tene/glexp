#version 330
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
smooth out vec4 gl_Color;
uniform float time;
void main()
{
    float ts = 3.14159f * 2.0f / 10;
    float ct = mod(time, 10);
    vec4 offset = vec4(
        cos(ct*ts)*0.5f,
        sin(ct*ts)*0.5f,
        0.0f,
        0.0f);

    gl_Position = position+offset;
    gl_Color = color;
}
