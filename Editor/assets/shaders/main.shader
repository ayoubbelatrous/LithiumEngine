#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TillingFactor;
layout(location = 4) in float a_TexIndex;
layout(location = 5) in float a_EntityID;


out vec4 Color;
out vec2 TexCoord;
out float v_TexIndex;
out float v_TillingFactor;
out flat float v_EntityID;

uniform mat4 u_projection;

void main()
{
	Color = a_Color;
	TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_EntityID = a_EntityID;
	v_TillingFactor = a_TillingFactor;
	gl_Position = u_projection * vec4(a_Position, 1.0);
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

uniform sampler2D u_textures[18];
in vec4 Color;
in vec2 TexCoord;
in float v_TexIndex;
in float v_TillingFactor;
in flat float v_EntityID;




void main()
{

	vec4 texColor = Color;
	switch (int(v_TexIndex))
	{
	case  0: texColor *= texture(u_textures[0], TexCoord * v_TillingFactor); break;
	case  1: texColor *= texture(u_textures[1], TexCoord * v_TillingFactor); break;
	case  2: texColor *= texture(u_textures[2], TexCoord * v_TillingFactor); break;
	case  3: texColor *= texture(u_textures[3], TexCoord * v_TillingFactor); break;
	case  4: texColor *= texture(u_textures[4], TexCoord * v_TillingFactor); break;
	case  5: texColor *= texture(u_textures[5], TexCoord * v_TillingFactor); break;
	case  6: texColor *= texture(u_textures[6], TexCoord * v_TillingFactor); break;
	case  7: texColor *= texture(u_textures[7], TexCoord * v_TillingFactor); break;
	case  8: texColor *= texture(u_textures[8], TexCoord * v_TillingFactor); break;
	case  9: texColor *= texture(u_textures[9], TexCoord * v_TillingFactor); break;
	case 10: texColor *= texture(u_textures[10], TexCoord * v_TillingFactor); break;
	case 11: texColor *= texture(u_textures[11], TexCoord * v_TillingFactor); break;
	case 12: texColor *= texture(u_textures[12], TexCoord * v_TillingFactor); break;
	case 13: texColor *= texture(u_textures[13], TexCoord * v_TillingFactor); break;
	case 14: texColor *= texture(u_textures[14], TexCoord * v_TillingFactor); break;
	case 15: texColor *= texture(u_textures[15], TexCoord * v_TillingFactor); break;
	case 16: texColor *= texture(u_textures[16], TexCoord * v_TillingFactor); break;
	case 17: texColor *= texture(u_textures[17], TexCoord * v_TillingFactor); break;

	}

	color = texColor;
	color2 = int(v_EntityID);
}