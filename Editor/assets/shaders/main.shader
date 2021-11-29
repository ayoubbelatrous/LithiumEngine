#shader vertex
#version 330 core

layout(location = 0) in vec2 pos;



void main()
{

    gl_Position = vec4(pos,1.0,1.0);
    
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 col;

uniform vec4 u_color;

void main()
{

   col = u_color;
};

