#shader vertex
#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 coords;

out vec2 vtex;

void main()
{
    vtex = coords;
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
    
}
#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 vtex;

uniform sampler2D utex;
uniform float exposure;
void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(utex, vtex).rgb;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));
    color = vec4(mapped, 1.0);
}