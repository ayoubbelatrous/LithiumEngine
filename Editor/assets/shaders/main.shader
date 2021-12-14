#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_EntityID;


out vec4 Color;
out vec2 TexCoord;
out float  v_TexIndex;
out flat float v_EntityID;

uniform mat4 u_projection;

void main()
{
	Color = a_Color;
	TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_EntityID = a_EntityID;
	gl_Position = u_projection * vec4(a_Position, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

uniform sampler2D u_textures[18];
in vec4 Color;
in vec2 TexCoord;
in float v_TexIndex;
in flat float v_EntityID;




void main()
{

	vec4 texColor = Color;
	switch (int(v_TexIndex))
	{
	case  0: texColor *= texture(u_textures[0], TexCoord); break;
	case  1: texColor *= texture(u_textures[1], TexCoord); break;
	case  2: texColor *= texture(u_textures[2], TexCoord); break;
	case  3: texColor *= texture(u_textures[3], TexCoord); break;
	case  4: texColor *= texture(u_textures[4], TexCoord); break;
	case  5: texColor *= texture(u_textures[5], TexCoord); break;
	case  6: texColor *= texture(u_textures[6], TexCoord); break;
	case  7: texColor *= texture(u_textures[7], TexCoord); break;
	case  8: texColor *= texture(u_textures[8], TexCoord); break;
	case  9: texColor *= texture(u_textures[9], TexCoord); break;
	case 10: texColor *= texture(u_textures[10], TexCoord); break;
	case 11: texColor *= texture(u_textures[11], TexCoord); break;
	case 12: texColor *= texture(u_textures[12], TexCoord); break;
	case 13: texColor *= texture(u_textures[13], TexCoord); break;
	case 14: texColor *= texture(u_textures[14], TexCoord); break;
	case 15: texColor *= texture(u_textures[15], TexCoord); break;
	case 16: texColor *= texture(u_textures[16], TexCoord); break;
	case 17: texColor *= texture(u_textures[17], TexCoord); break;

	}

	color = texColor;
	color2 = int(v_EntityID);
}