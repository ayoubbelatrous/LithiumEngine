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

void main()
{
    //vec4 test = vec4(vtex.x, vtex.y, 0.0, 1.0);

   color = texture(utex,vtex);
  // color = vec4(vec3(1.0 - texture(utex, vtex)), 1.0);
}