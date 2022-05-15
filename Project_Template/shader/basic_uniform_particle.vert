#version 460

layout (location = 0) in vec3 VertexInitVel; 
layout (location = 1) in float VertexBirthTime; 

uniform int Pass;

out float Transp; // Particle transparancy
out vec2 TexCoord;

uniform float Time; 
uniform vec3 Gravity = vec3(0.0,-0.05,0.0); 
uniform float ParticleLifetime; 
uniform float ParticleSize = 1;
uniform vec3 EmitterPos; // emitter position, use to manipulate the direction it is going int

uniform mat4 MV;
uniform mat4 ProjectionMatrix;

// offsets to the position in camera coordinates for each vertex of particles
const vec3 offsets[] = vec3[](vec3(-0.5,-0.5,0),vec3(0.5,-0.5,0),vec3(0.5,0.5,0),
                              vec3(-0.5,-0.5,0),vec3(0.5,0.5,0),vec3(-0.5,0.5,0));

// texture coordinates for each vertex of the particles quad
const vec2 texCoords[] = vec2[](vec2(0,0),vec2(1,0),vec2(1,1),vec2(0,0),vec2(1,1),vec2(0,1));

void main()
{
    vec3 cameraPos; 
    float t = Time - VertexBirthTime;
    if(t>=0 && t<ParticleLifetime)
    {
        vec3 pos = EmitterPos + VertexInitVel * t + Gravity * t * t;

       
        cameraPos = (MV * vec4(pos,1)).xyz + (offsets[gl_VertexID] * ParticleSize);
        Transp = mix(1,0,t/ParticleLifetime);
    } else {
        // For transparancy
        cameraPos = vec3(0);
        Transp = 0.0;
    }

    TexCoord = texCoords[gl_VertexID];

   
    gl_Position = ProjectionMatrix * vec4(cameraPos,1);
}