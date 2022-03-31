#version 460

//in variable that receives the diffuse calculation from the vertex shader
in vec3 Colour;
in vec2 TexCoord;
in vec3 Position;
in vec3 Normal;
//out variable, this typical for all fragment shaders
layout (location = 0) out vec4 FragColor;
layout(binding=0) uniform sampler2D Tex1;
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
vec3 texColor = texture(Tex1, TexCoord).rgb;
vec3 ambient = texColor * light.la;

vec3 s = vec3 (0.0);
s = normalize (vec3 (light.position) - position); //calculating diffuse 

float sdotn = max (dot(s, normal), 0.0);

vec3 diffuse = texColor * sdotn;

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
    //we pass LightInyensity to outr FragColor, notice the difference between vector types
    // vec3 and vec4 and how we solved the problem
    FragColor = vec4(blinnPhong(Position, normalize(Normal)), 1.0);

   
}
