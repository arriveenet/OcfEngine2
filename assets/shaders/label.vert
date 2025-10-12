#version 330

// Uniforms for world transform and view-proj
uniform mat4 uMVPMatrix;

// Attribute 0 is position, 1 is normal, 2 is tex coords.
layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;

// Any vertex outputs (other than position)
out vec2 fragTexCoord;
out vec3 fontColor;

void main()
{
	// Convert position to homogeneous coordinates
	vec4 pos = vec4(inPosition, 1.0);
	// Transform to position world space, then clip space
	gl_Position = uMVPMatrix * pos;

	// Pass along the texture coordinate to frag shader
	fragTexCoord = inTexCoord;
	fontColor = inColor;
}
