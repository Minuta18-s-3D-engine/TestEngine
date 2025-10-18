#version 430
out vec4 FragColor;

#define MATH_EPS 0.00001

struct Cluster {
    vec4 minPoint;
    vec4 maxPoint;
    uint count;
    uint lightIndices[100];
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    float shininess;
};

struct TextureMaterial {
    float scaleX, scaleY;
    float shiftX, shiftY;
    bool isActive;
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
uniform int numLights;

uniform float zNear;
uniform float zFar;
uniform mat4 inverseProjection;
uniform uvec3 gridSize;
uniform uvec2 screenDimensions;
uniform vec3 ViewPos;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;
in vec3 ViewSpacePos;

uniform Material mainMaterial;

uniform TextureMaterial textureDiffuse1Mat;
uniform TextureMaterial textureSpecular1Mat;
uniform sampler2D textureDiffuse1;
uniform sampler2D textureSpecular1;

const float AMBIENT_LIGHT = 0.3;

vec3 calcAmbientLight(vec2 coords) {
    return texture(textureDiffuse1, coords).rgb * AMBIENT_LIGHT;
}

uint findClusterIndex() {
    uvec2 tileSize = screenDimensions / gridSize.xy;
    uvec2 tileXY = uvec2(gl_FragCoord.xy) / tileSize;
    tileXY = min(tileXY, gridSize.xy - 1u);

    float viewZ = -ViewSpacePos.z;
    uint tileZ = uint((log(viewZ / zNear) * gridSize.z) / log(zFar / zNear));
    tileZ = clamp(tileZ, 0u, gridSize.z - 1u);

    return tileXY.x + tileXY.y * gridSize.x + tileZ * gridSize.x * gridSize.y;
}

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 calcPointLight(PointLight pLight) {
    // PointLight pLight = lights[lightId];
    vec3 viewDir  = normalize(ViewPos - FragPos);
    vec3 lightDir = normalize(pLight.position - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); 

    vec3 albedo = texture(textureDiffuse1, TexCoords).rgb;
    float specular = texture(textureSpecular1, TexCoords).r;

    float spec = pow(max(dot(Normal, halfwayDir), 0.0), mainMaterial.shininess);

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

void main() {
    uint clusterIndex = findClusterIndex();
    Cluster currCluster = clusters[clusterIndex];

    uint totalClusters = gridSize.x * gridSize.y * gridSize.z;
    if (clusterIndex >= totalClusters) {
        return;
    }

    vec3 result = calcAmbientLight(TexCoords);
    uint lightCount = currCluster.count;
    for (int i = 0; i < lightCount; ++i) {
        uint lightIndex = currCluster.lightIndices[i];
        PointLight l = lights[lightIndex]; 
        // float dist = length(lights[lightIndex].position - FragPos);

        // if (dist < lights[lightIndex].radius) {
            result += calcPointLight( l);
        // }
    }

    // if (dist < lights[lightIndex].radius) {
        // result += calcPointLight(lightIndex);
    // }

    FragColor = vec4(result, 1.0); 
}
