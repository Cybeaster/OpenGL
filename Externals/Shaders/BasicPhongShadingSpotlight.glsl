#shader vertex
#version 430
layout(location=0) in vec3 vertPosition;
layout(location=1) in vec2 texels;
layout(location=2) in vec3 normal;

out vec3 varyingNormal;
out vec3 varyingLightDir;
out vec3 varyingHalfVector;
out vec3 varyingVertPos;
out vec4 shadowCoord;
out vec2 tc;

struct AttenuationFactor
{
    float constant;
    float quadratic;
    float linear;
};

struct LightBase
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct DirectionalLight
{
    LightBase base;
    vec3 direction;
};

struct PositionalLight
{
    LightBase base;
    vec3 position;
    AttenuationFactor attenuation;
};

struct Spotlight
{
    PositionalLight base;
    vec3 direction;
    float cutoff;
};

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};


uniform vec4 globalAmbient;
uniform Spotlight light;
uniform Material material;
uniform mat4 projMatrix;
uniform mat4 mvMatrix;
uniform mat4 normMatrix;

uniform int useTexture = 1;

uniform mat4 shadowMVP;

layout (binding = 0) uniform sampler2DShadow shadowTexture;

void main(void)
{
    varyingVertPos = (mvMatrix * vec4(vertPosition, 1.0)).xyz;

    varyingLightDir = light.base.position - varyingVertPos;
    varyingHalfVector = (varyingLightDir + (-varyingVertPos)).xyz;

    shadowCoord = shadowMVP * vec4(vertPosition, 1.0);

    varyingNormal = (normMatrix * vec4(normal, 1.0)).xyz;
    gl_Position = projMatrix * mvMatrix * vec4(vertPosition, 1.0);
    tc = texels;
}




#shader fragment
#version 430

in vec2 tc;
in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingHalfVector;
in vec3 varyingVertPos;
in vec4 shadowCoord;

out vec4 fragColor;

layout (binding = 0) uniform sampler2DShadow shadowTexture;


struct AttenuationFactor
{
    float constant;
    float quadratic;
    float linear;
};

struct LightBase
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct DirectionalLight
{
    LightBase base;
    vec3 direction;
};

struct PositionalLight
{
    LightBase base;
    vec3 position;
    AttenuationFactor attenuation;
};

struct Spotlight
{
    PositionalLight base;
    vec3 direction;
    float cutoff;
};

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};


uniform vec4 globalAmbient;
uniform Spotlight light;
uniform Material material;
uniform mat4 projMatrix;
uniform mat4 mvMatrix;
uniform mat4 normMatrix;
uniform int useTexture = 1;

vec4 CalcLightInternal(LightBase Light, vec3 Direction, vec3 Normal)
{
    vec3 L = normalize(varyingLightDir);
    vec3 N = normalize(Normal);
    vec3 H = normalize(varyingHalfVector);

    float diffuseFactor = max(dot(L, N), 0.0);// perfomance update
    float specularFactor = max(dot(H, N), 0.0);

    vec4 ambientColor = ((globalAmbient * material.ambient) + (Light.ambient * material.ambient));
    vec4 diffuse = Light.diffuse * material.diffuse * diffuseFactor;
    vec4 specular = Light.specular * material.specular * pow(specularFactor, material.shininess * 3.0);

    //vec4 texColor = texture(sampler, tc);
    float notInShadow = textureProj(shadowTexture, shadowCoord);
    if (notInShadow == 1.0)
    {
        return (ambientColor + diffuse + specular);
    }
    else
    {
        return ambientColor;
    }
}


vec4 CalcPointLight(PositionalLight Light, vec3 Normal)
{
    vec3 dir = varyingVertPos - Light.position;
    ///float distance = length(dir);
    dir = normalize(dir);

    //float attenuation = (Light.attenuation.constant + Light.attenuation.linear * distance + Light.attenuation.quadratic * (distance * distance));

    vec4 color = CalcLightInternal(Light.base, dir, Normal);
    return color;/// attenuation;
}

vec4 CalcDirectionalLight(DirectionalLight Light, vec3 Normal)
{
    return CalcLightInternal(Light.base, Light.direction, Normal);
}

vec4 CalcSpotLight(Spotlight Light, vec3 Normal)
{
    //vec3 lightToPixel = normalize(varyingVertPos -  Light.base.position);
    //float spotFactor = dot(lightToPixel, Light.direction);

    vec4 color = CalcPointLight(Light.base, Normal);
    // float spotLightIntensity = (1.0 - (1.0 - spotFactor) / (1.0 - Light.cutoff));
    return color;//* (spotLightIntensity + globalAmbient);

}


void main(void)
{
    fragColor = CalcSpotLight(light, varyingNormal);
}