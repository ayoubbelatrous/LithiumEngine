#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_tex;

layout(std140) uniform Camera
{
	mat4 projection;
};
out vec2 v_tex;
uniform mat4 model;
void main()
{
	v_tex = a_tex;
	gl_Position = projection * model * vec4(a_Position,1.0) ;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;



in vec2 v_tex;


uniform vec4 MainColor;
uniform sampler2D tex;

void main()
{


	color =  MainColor * texture(tex,v_tex);
}