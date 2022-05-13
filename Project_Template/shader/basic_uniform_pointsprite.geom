#version 430

// Define recieved primitives (points)
layout(points) in;

// We want to output triangle stip primitive types
// Strip has max vertices 4, 2 triangles to made up a quad
layout(triangle_strip, max_vertices = 4) out;

// Half width of quad
uniform float Size2;

uniform mat4 ProjectionMatrix;

out vec2 TexCoord;

void main()
{
	// Apply projection
	mat4 m = ProjectionMatrix;

	// Setup vertex 1
	gl_Position = m * (vec4(-Size2, -Size2, 0.0, 0.0) + gl_in[0].gl_Position);

	TexCoord = vec2(0.0, 0.0);
	EmitVertex();

	// Setup vertex 2
	gl_Position = m * (vec4(Size2, -Size2, 0.0, 0.0) + gl_in[0].gl_Position);

	TexCoord = vec2(1.0, 0.0);
	EmitVertex();

	// Setup vertex 3
	gl_Position = m * (vec4(-Size2, Size2, 0.0, 0.0) + gl_in[0].gl_Position);

	TexCoord = vec2(0.0, 1.0);
	EmitVertex();

	// Setup vertex 4
	gl_Position = m * (vec4(Size2, Size2, 0.0, 0.0) + gl_in[0].gl_Position);

	TexCoord = vec2(1.0, 1.0);
	EmitVertex();

	// Finalize primitive to send it along pipeline
	EndPrimitive();
}