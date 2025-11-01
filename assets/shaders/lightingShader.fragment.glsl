#version 430 core
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

in vec2 TexCoords;
uniform vec3 viewPos;

struct Cluster {
    vec4 minPoint;
    vec4 maxPoint;
    uint count;
    uint lightStart;
};


struct PointLight {
    vec3 position;
    vec3 color;
    
    float linear;
    float quadratic;
    float radius;
};

layout (std430, binding = 0) restrict buffer clusterSSBO {
    Cluster clusters[];
};

layout (std430, binding = 1) restrict buffer lightSSBO {
    PointLight lights[];
};

layout(std430, binding = 2) restrict buffer lightIndicesSSBO {
    uint pointLightIndicies[];
};

vec3 FragPos = texture(gPosition, TexCoords).rgb;
vec3 Normal = texture(gNormal, TexCoords).rgb;

vec3 calcPointLight(PointLight pLight) {
    vec3 pathToLight = pLight.position - FragPos;
    float distSq = dot(pathToLight, pathToLight);
    float radiusSq = pLight.radius * pLight.radius;

    if (distSq > radiusSq) return vec3(0.0);

    vec3 viewDir  = normalize(ViewPos - FragPos);
    vec3 lightDir = normalize(pLight.position - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); 

    vec3 albedo = texture(gAlbedoSpec, coords).rgb;
    float specular = texture(gAlbedoSpec, coords).a;;

    // TODO: shinessness will go here
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16);

    vec3 diffuseLighting = max(dot(Normal, lightDir), 0.0) 
        * albedo * pLight.color;
    vec3 specularLighting = spec * specular * pLight.color;

    float distToLightSource = length(pLight.position - FragPos);
    float linearPart = pLight.linear * distToLightSource;
    float quadraticPart = pLight.quadratic * distToLightSource 
        * distToLightSource;
    float attenuation = 1.0 / (0.7 + linearPart + quadraticPart);

    return (diffuseLighting + specularLighting) * attenuation;
}

const float AMBIENT_LIGHT = 0.05;

vec3 calcAmbientLight(vec2 coords) {
    return texture(gAlbedoSpec, coords).rgb * AMBIENT_LIGHT;
}

void main() {
    vec3 result = calcAmbientLight(TexCoords);

    for (int i = 0; i < 100; ++i) {
        PointLight l = lights[i]; 
        
        result += calcPointLight();
    }

    FragColor = vec4(result, 1.0);
}
