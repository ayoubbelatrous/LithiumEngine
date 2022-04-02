#shader vertex
#version 450 core

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
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

uniform sampler2D u_textures[18];
in vec4 Color;
in vec2 TexCoord;
in float v_TexIndex;
in flat float v_EntityID;
uniform float pxRange;


float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

float screenPxRange(ivec2 texsize) {
	vec2 unitRange = vec2(pxRange) / vec2(texsize);
	vec2 screenTexSize = vec2(1.0) / fwidth(TexCoord);
	return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

void main()
{

	vec3 msdColor;
	ivec2 TextureSize;
	switch (int(v_TexIndex))
	{
	case  0: {msdColor = texture(u_textures[0], TexCoord).rgb;  TextureSize = textureSize(u_textures[0], 0); } break;
	case  1: {msdColor = texture(u_textures[1], TexCoord).rgb; TextureSize = textureSize(u_textures[1], 0); } break;
	case  2: {msdColor = texture(u_textures[2], TexCoord).rgb; TextureSize = textureSize(u_textures[2], 0); } break;
	case  3: {msdColor = texture(u_textures[3], TexCoord).rgb; TextureSize = textureSize(u_textures[3], 0); } break;
	case  4: {msdColor = texture(u_textures[4], TexCoord).rgb; TextureSize = textureSize(u_textures[4], 0); } break;
	case  5: {msdColor = texture(u_textures[5], TexCoord).rgb; TextureSize = textureSize(u_textures[5], 0); } break;
	case  6: {msdColor = texture(u_textures[6], TexCoord).rgb; TextureSize = textureSize(u_textures[6], 0); } break;
	case  7: {msdColor = texture(u_textures[7], TexCoord).rgb; TextureSize = textureSize(u_textures[7], 0); } break;
	case  8: {msdColor = texture(u_textures[8], TexCoord).rgb; TextureSize = textureSize(u_textures[8], 0); } break;
	case  9: {msdColor = texture(u_textures[9], TexCoord).rgb; TextureSize = textureSize(u_textures[9], 0); } break;
	case 10: {msdColor = texture(u_textures[10], TexCoord).rgb; TextureSize = textureSize(u_textures[10], 0); } break;
	case 11: {msdColor = texture(u_textures[11], TexCoord).rgb; TextureSize = textureSize(u_textures[11], 0); } break;
	case 12: {msdColor = texture(u_textures[12], TexCoord).rgb; TextureSize = textureSize(u_textures[12], 0); } break;
	case 13: {msdColor = texture(u_textures[13], TexCoord).rgb; TextureSize = textureSize(u_textures[13], 0); } break;
	case 14: {msdColor = texture(u_textures[14], TexCoord).rgb; TextureSize = textureSize(u_textures[14], 0); } break;
	case 15: {msdColor = texture(u_textures[15], TexCoord).rgb; TextureSize = textureSize(u_textures[15], 0); } break;
	case 16: {msdColor = texture(u_textures[16], TexCoord).rgb; TextureSize = textureSize(u_textures[16], 0); } break;
	case 17: {msdColor = texture(u_textures[17], TexCoord).rgb; TextureSize = textureSize(u_textures[17], 0);} break;

	}
	float sd = median(msdColor.r, msdColor.g, msdColor.b);
	float screenPxDistance = screenPxRange(TextureSize) * (sd - 0.5);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

	color = mix(vec4(0.5f,0.5f, 0.51f, 1.0f), Color, opacity);
	
	color2 = int(v_EntityID);
}

