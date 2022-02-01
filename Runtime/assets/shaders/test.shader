#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;


uniform mat4 u_projection;
uniform mat4 model;
out vec2 v_tex;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	
	gl_Position = u_projection * vec4(a_Position, 1.0);
	v_tex = a_UV;
	FragPos = vec3(model * vec4(a_Position, 1.0));
	Normal = a_Normal;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D diffuse;
uniform vec3 lightPos;

in vec2 v_tex;
in vec3 Normal;
in vec3 FragPos;

void main()
{
	vec3 objectColor = texture(diffuse, v_tex).rgb;
	vec3 ambient = vec3(0.1);
	vec3 lightColor = vec3(1.0);
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir * 2), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 result = (ambient + diffuse) * objectColor;
	color = vec4(result, 1.0);
	
}