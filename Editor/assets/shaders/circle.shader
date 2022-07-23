#shader vertex
#version 450 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;
layout(location = 5) in int a_EntityID;

uniform mat4 u_projection;



out vec3 LocalPosition;
out vec4 Color;
out float Thickness;
out float Fade;


out flat int v_EntityID;

void main()
{
	LocalPosition = a_LocalPosition;
	Color = a_Color;
	Thickness = a_Thickness;
	Fade = a_Fade;

	v_EntityID = a_EntityID;

	gl_Position = u_projection * vec4(a_WorldPosition, 1.0);
}

#shader fragment
#version 450 core

out vec4 o_Color;
out int o_EntityID;


in vec3 LocalPosition;
in vec4 Color;
in float Thickness;
in float Fade;

in flat int v_EntityID;

void main()
{

	float distance = 1.0 - length(LocalPosition);
	float circle = smoothstep(0.0, Fade, distance);
	circle *= smoothstep(Thickness + Fade, Thickness, distance);

	if (circle == 0.0)
		discard;
	o_Color = Color;
	o_Color.a *= circle;

	o_EntityID = v_EntityID;
}