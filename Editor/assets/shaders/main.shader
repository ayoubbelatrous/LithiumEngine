#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;



uniform mat4 u_projection;


out vec4 Color;
out vec2 TexCoord;
out float v_TexIndex;
out float TilingFactor;

void main()
{
	Color = a_Color;
	TexCoord = a_TexCoord;
	TilingFactor = a_TilingFactor;
	v_TexIndex = a_TexIndex;

	gl_Position = u_projection * vec4(a_Position, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
uniform sampler2D u_Textures[32];
in vec4 Color;
in vec2 TexCoord;
in float v_TexIndex;
in float TilingFactor;

void main()
{
	vec4 texColor = Color;

	switch (int(v_TexIndex))
	{
	case  0: texColor *= texture(u_Textures[0], TexCoord * TilingFactor); break;
	case  1: texColor *= texture(u_Textures[1], TexCoord * TilingFactor); break;
	case  2: texColor *= texture(u_Textures[2], TexCoord * TilingFactor); break;
	case  3: texColor *= texture(u_Textures[3], TexCoord * TilingFactor); break;
	case  4: texColor *= texture(u_Textures[4], TexCoord * TilingFactor); break;
	case  5: texColor *= texture(u_Textures[5], TexCoord * TilingFactor); break;
	case  6: texColor *= texture(u_Textures[6], TexCoord * TilingFactor); break;
	case  7: texColor *= texture(u_Textures[7], TexCoord * TilingFactor); break;
	case  8: texColor *= texture(u_Textures[8], TexCoord * TilingFactor); break;
	case  9: texColor *= texture(u_Textures[9], TexCoord * TilingFactor); break;
	case 10: texColor *= texture(u_Textures[10], TexCoord * TilingFactor); break;
	case 11: texColor *= texture(u_Textures[11], TexCoord * TilingFactor); break;
	case 12: texColor *= texture(u_Textures[12], TexCoord * TilingFactor); break;
	case 13: texColor *= texture(u_Textures[13], TexCoord * TilingFactor); break;
	case 14: texColor *= texture(u_Textures[14], TexCoord * TilingFactor); break;
	case 15: texColor *= texture(u_Textures[15], TexCoord * TilingFactor); break;
	case 16: texColor *= texture(u_Textures[16], TexCoord * TilingFactor); break;
	case 17: texColor *= texture(u_Textures[17], TexCoord * TilingFactor); break;
	case 18: texColor *= texture(u_Textures[18], TexCoord * TilingFactor); break;
	case 19: texColor *= texture(u_Textures[19], TexCoord * TilingFactor); break;
	case 20: texColor *= texture(u_Textures[20], TexCoord * TilingFactor); break;
	case 21: texColor *= texture(u_Textures[21], TexCoord * TilingFactor); break;
	case 22: texColor *= texture(u_Textures[22], TexCoord * TilingFactor); break;
	case 23: texColor *= texture(u_Textures[23], TexCoord * TilingFactor); break;
	case 24: texColor *= texture(u_Textures[24], TexCoord * TilingFactor); break;
	case 25: texColor *= texture(u_Textures[25], TexCoord * TilingFactor); break;
	case 26: texColor *= texture(u_Textures[26], TexCoord * TilingFactor); break;
	case 27: texColor *= texture(u_Textures[27], TexCoord * TilingFactor); break;
	case 28: texColor *= texture(u_Textures[28], TexCoord * TilingFactor); break;
	case 29: texColor *= texture(u_Textures[29], TexCoord * TilingFactor); break;
	case 30: texColor *= texture(u_Textures[30], TexCoord * TilingFactor); break;
	case 31: texColor *= texture(u_Textures[31], TexCoord * TilingFactor); break;
	}
	color = texColor;

}