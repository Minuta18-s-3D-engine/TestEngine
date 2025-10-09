#version 430
out vec4 FragColor;

struct Cluster {
    vec4 minPoint;
    vec4 maxPoint;
    uint count;
    uint lightIndices[100];
};

struct GPUPointLight {
    vec4 position;
    vec4 color;
};

layout (std430, binding = 1) restrict buffer clusterSSBO {
    Cluster clusters[];
}

layout (std430, binding = 2) restrict buffer lightSSBO {
    GPUPointLight GPULights[];
}

struct Light {
    vec3 position;
    vec3 color;
    
    float linear;
    float quadratic;
    float radius;
};

const int MAX_NR_LIGHTS = 1224;
uniform Light lights[MAX_NR_LIGHTS];
uniform int lights_size;

uniform float zNear;
uniform float zFar;
uniform mat4 inverseProjection;
uniform uvec3 gridSize;
uniform uvec2 screenDimensions;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;

uniform vec3 viewPos;
uniform int numLights;
uniform Material mainMaterial;

uniform TextureMaterial textureDiffuse1Mat;
uniform TextureMaterial textureSpecular1Mat;
uniform sampler2D textureDiffuse1;
uniform sampler2D textureSpecular1;

vec3 getDiffTexturePixel(vec2 coords) {
    if (!textureDiffuse1Mat.isActive) return vec3(0.0, 0.0, 0.0);
    return vec3(texture(textureDiffuse1, coords));
}

vec3 getSpecTexturePixel(vec2 coords) {
    if (!textureSpecular1Mat.isActive) return vec3(0.0, 0.0, 0.0);
    return vec3(texture(textureSpecular1, coords));
}

const float AMBIENT_LIGHT = 0.05;

vec3 calcAmbientLight(vec2 coords) {
    return texture(textureDiffuse1, coords).rgb * AMBIENT_LIGHT;
}

vec3 calcDiffuseLight(int lightId, vec2 coords) {
    vec3 Diffuse = texture(textureDiffuse1, coords).rgb;
    vec3 lightDir = normalize(lights[lightId].position - FragPos);
    return max(dot(Normal, lightDir), 0.0) * Diffuse * lights[lightId].color;
}

vec3 calcSpecularLight(int lightId, vec2 coords) {
    float Specular = texture(textureSpecular1, coords).r;
    vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(lights[lightId].position - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0); // shinessness 
            // will go here
    vec3 specular = lights[lightId].color * spec * Specular;
    return specular;
}

float calcAttenuation(int lightId, float distToLightSource) {
    float linearPart = lights[lightId].linear * distToLightSource;
    float quadraticPart = lights[lightId].quadratic * distToLightSource 
        * distToLightSource;
    float attenuation = 1.0 / (0.7 + linearPart + quadraticPart);
    return attenuation;
}

uint findClusterIndex() {
    uvec2 tileSize = screenDimensions / gridSize.xy;
    uvec2 tileXY = uvec2(gl_FragCoord.xy) / tileSize;
    tileXY = min(tileXY, gridSize.xy - 1u);

    float viewZ = -FragPos.z;
    uint tileZ = uint((log(viewZ / zNear) * gridSize.z) / log(zFar / zNear));
    tileZ = clamp(tileZ, 0u, gridSize.z - 1u);

    return tileXY.x + tileXY.y * gridSize.x + tileZ * gridSize.x * gridSize.y;
}

void main() {
    uint clusterIndex = findClusterIndex();
    Cluster currCluster = clusters[clusterIndex];
    
    vec3 result = calcAmbientLight(TexCoords);
    for (int i = 0; i < cluster.count; ++i) {
        uint lightIndex = cluster.lightIndices[i];

        float dist = length(GPULights[lightIndex].position.xyz - FragPos);

        if (dist < GPULights[lightIndex].position.w) {
            vec3 diffuse = calcDiffuseLight(lightIndex);
            vec3 specular = calcSpecularLight(lightIndex);
            float attenuation = calcAttenuation(lightIndex, dist);

            result += (diffuse + specular) * attenuation;
        }
    }

    FragColor = vec4(result, 1.0);
}

