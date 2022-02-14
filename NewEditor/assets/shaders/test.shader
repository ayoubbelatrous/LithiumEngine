#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;


uniform vec4 v4;
uniform vec2 vert;
out vec2 v_vert;


void main()
{
	
	v_vert = vert;
	gl_Position = v4;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;


in vec2 v_vert;
uniform sampler2D u_tex;

void main()
{



	color = texture(u_tex, v_vert);

}