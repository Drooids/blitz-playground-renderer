#version 330 core

out vec4 FragColor;
in vec4 vertexColor;

in vec4 outPosition;

uniform vec4 ourColor;

void main()
{
	FragColor = outPosition;
}
