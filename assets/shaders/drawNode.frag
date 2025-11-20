#version 330

// Tex coord input from vertex shader
in vec4 fragColor;

// This corresponds to the output color to the color buffer
out vec4 outColor;

void main()
{
	outColor = fragColor;
}