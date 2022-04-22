#version 460

//in variable that receives the diffuse calculation from the vertex shader

in vec3 Position;
in vec3 Normal;


//out variable, this typical for all fragment shaders
layout (location = 0) out vec4 FragColor;
layout(binding=0) uniform sampler2D RenderTex;




//skybox
//layout(binding=4) uniform samplerCube SkyBoxTex;


//in vec3 Vec;
out vec4 FragColour;


uniform struct lightInfo
{
vec4 position;
vec3 la;
vec3 l;
} light;


uniform struct lightMaterialInfo
{
vec3 ka;
vec3 kd;
vec3 ks;
float shininess;
} material;

uniform float edgeThreshold; //declaring uniform for the edge freshold
uniform int pass; 
const vec3 lum = vec3(0.2126, 0.7152, 0.0722);

vec3 blinnPhong (vec3 position, vec3 normal)
{





vec3 ambient = material.ka * light.la; //replacing texmix with material.ka

vec3 s = vec3 (0.0);
s = normalize (vec3 (light.position) - position); //calculating diffuse 

float sdotn = max (dot(s, normal), 0.0);

vec3 diffuse = material.kd * sdotn; //replace texmix with material.kd

vec3 spec = vec3 (0.0);

if (sdotn > 0.0)
{
vec3 v = normalize (-position.xyz);

vec3 h = normalize (v + s);

spec = material.ks * pow(max(dot(h, normal), 0.0), material.shininess);
}
return ambient + light.l *(diffuse + spec);
}

float luminance( vec3 color )
{
 return dot(lum,color);
}



vec4 pass1()
{
 return vec4(blinnPhong( Position, normalize(Normal) ),1.0);
}

vec4 pass2()
{
 ivec2 pix = ivec2(gl_FragCoord.xy); //we grab a pixel to check if edge
//pick neighboutring pixels for convolution filter
//check lecture slides
 float s00 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1,1)).rgb);
 float s10 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1,0)).rgb);
 float s20 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1,-1)).rgb);
 float s01 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(0,1)).rgb);
 float s21 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(0,-1)).rgb);
 float s02 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(1,1)).rgb);
 float s12 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(1,0)).rgb);
 float s22 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(1,-1)).rgb);
 float sx = s00 + 2 * s10 + s20 - (s02 + 2 * s12 + s22);
 float sy = s00 + 2 * s01 + s02 - (s20 + 2 * s21 + s22);
 float g = sx * sx + sy * sy;
 if( g > edgeThreshold )
 return vec4(1.0); //edge
 else
 return vec4(0.0,0.0,0.0,1.0); //no edge
}



void main()
{
vec3 FragmentColour;

    //we pass LightInyensity to outr FragColor, notice the difference between vector types
    // vec3 and vec4 and how we solved the problem
    FragmentColour = blinnPhong(Position, normalize(Normal));

  if( pass == 1 ) FragColor = pass1();
 if( pass == 2 ) FragColor = pass2();

   
}
