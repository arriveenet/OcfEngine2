#version 330

// Tex coord input from vertex shader
in vec2 fragTexCoord;
in vec3 fontColor;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

void main()
{
	float alpha = texture(uTexture, fragTexCoord).r;
	outColor = vec4(fontColor, alpha);
}
