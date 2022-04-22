#version 460

//in variable that receives the diffuse calculation from the vertex shader
in vec3 Colour;
in vec2 TexCoord;
in vec3 Position;
in vec3 Normal;
in vec4 projectionTexCoord;
in vec3 vec;

//out variable, this typical for all fragment shaders
layout (location = 0) out vec4 FragColor;
layout(binding=0) uniform sampler2D Tex1;

//multi tex
layout(binding=0) uniform sampler2D BrickTex;
layout(binding=1) uniform sampler2D MossTex;


//skybox
layout(binding=4) uniform samplerCube SkyBoxTex;


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


vec3 blinnPhong (vec3 position, vec3 normal)
{
vec4 texColor = texture(Tex1, TexCoord); //extract texture
vec4 texColor2 = texture(MossTex, TexCoord); //extract textures to mix them together

vec3 texMix = mix(texColor.rgb, texColor2.rgb, texColor2.a); //mixes the fields together in order to create a mixed texture


vec3 ambient = texMix * light.la;

vec3 s = vec3 (0.0);
s = normalize (vec3 (light.position) - position); //calculating diffuse 

float sdotn = max (dot(s, normal), 0.0);

vec3 diffuse = texMix * sdotn;

vec3 spec = vec3 (0.0);

if (sdotn > 0.0)
{
vec3 v = normalize (-position.xyz);

vec3 h = normalize (v + s);

spec = material.ks * pow(max(dot(h, normal), 0.0), material.shininess);
}
return ambient + light.l *(diffuse + spec);
}

void main()
{
vec3 FragmentColour;

    //we pass LightInyensity to outr FragColor, notice the difference between vector types
    // vec3 and vec4 and how we solved the problem
    FragmentColour = blinnPhong(Position, normalize(Normal));

    vec3 texColor = texture(SkyBoxTex, normalize(vec)).rgb; //how to assign to frag colour?
   FragColor = vec4(FragmentColour + texColor *1.5, 1);
   
}
