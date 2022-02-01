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

uniform sampler2D u_tex;

void main()
{


   color = texture(u_tex,vtex);
   float gamma = 2.2;
   color.rgb = pow(color.rgb, vec3(1.0 / gamma));
   
}