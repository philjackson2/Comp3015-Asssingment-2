#version 430
layout( points ) in; //define the type of primitives recived 
layout ( triangle_strip, max_vertices = 4 ) out; //the primitives produced 

uniform float Size2; 

uniform mat4 ProjectionMatrix; 

out vec2 TexCoord; 


void main()
{
mat4 m = ProjectionMatrix; 


//vertex 1
gl_Position = m * (vec4(-Size2, -Size2,0.0,0.0) + gl_in[0].gl_Position); 

TexCoord = vec2(0.0,0.0); 
EmitVertex();


//vertex 2
gl_Position = m * (vec4(Size2, -Size2, 0.0,0.0) + gl_in[0].gl_Position); 
TexCoord = vec2(1.0,0.0);

EmitVertex(); 

//vertex 3

gl_Position = m * (vec4(-Size2,Size2,0.0,0.0) + gl_in[0].gl_Position);
TexCoord = vec2(0.0,1.0);
EmitVertex();

//vertex 4
gl_Position = m * (vec4(Size2,Size2,0.0,0.0) + gl_in[0].gl_Position);
TexCoord = vec2(1.0,1.0); 
EmitVertex();


EndPrimitive(); //finalises the primative 
}