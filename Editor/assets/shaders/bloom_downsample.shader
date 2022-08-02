#shader vertex
#version 450 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_Coords;

out vec2 v_tex;

void main()
{
    v_tex = a_Coords;
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
}

#shader fragment
#version 450 core

layout(location = 0) out vec3 color;

in vec2 v_tex;
uniform sampler2D u_Input;
uniform float u_Threshold;

void main()
{
    vec3 inputColor = texture(u_Input, v_tex).rgb;
    inputColor.r -= u_Threshold;
    inputColor.g -= u_Threshold;
    inputColor.b -= u_Threshold;
    color = inputColor;
}
