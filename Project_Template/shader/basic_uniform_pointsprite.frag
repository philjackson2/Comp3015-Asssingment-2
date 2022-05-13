#version 430

// Texture coords from geometry shader
in vec2 TexCoord;

uniform sampler2D SpriteTex;

layout(location = 0) out vec4 FragColour;

void main()
{
	// Assign colour from texture
	FragColour = texture(SpriteTex, TexCoord);
}