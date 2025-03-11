#version 330 core
out vec4 FragColor;

in vec2 TexCoords;


uniform sampler2D floorTexCoords;

void main()
{
	FragColor=texture(floorTexCoords,TexCoords);
}
