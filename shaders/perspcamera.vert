#version 330
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
smooth out vec4 gl_Color;

uniform vec3 offset;
uniform mat4 camera;
uniform mat4 perspective;

void main()
{
    vec4 cp = position + vec4(offset.x, offset.y, offset.z, 0.0);
    gl_Position = camera * cp;
    gl_Position = perspective * gl_Position;
    gl_Color = color;
}
