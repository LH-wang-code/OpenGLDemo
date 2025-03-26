#version 330 core

layout (location = 0) in vec3 position;
layout (location = 3) in mat4 aInstanceMatrix;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

uniform bool isIstanced;//判断是否实例化

void main()
{
	if(isIstanced)
		gl_Position=lightSpaceMatrix * aInstanceMatrix * vec4(position,1.0f);
	else
		gl_Position = lightSpaceMatrix * model * vec4(position,1.0f);
}