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
	int index = int(v_TexIndex);
	color = Color * texture(u_textures[index], TexCoord);
	color2 = int(v_EntityID);
}