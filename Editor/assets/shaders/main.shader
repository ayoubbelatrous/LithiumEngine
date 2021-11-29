#shader vertex
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float textureIndex;

out vec4 Color;
out vec2 texcoord;
out float texindex;

uniform mat4 viewProjection;

void main()
{
    Color = color;
    texcoord = texCoord;
    texindex = textureIndex;
    gl_Position = viewProjection * vec4(pos,1.0) ;
    
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 col;


in vec4 Color;
in vec2 texcoord;
in float texindex;

void main()
{

   col = Color;
};

