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

vec3 getDiffTexturePixel(vec2 coords) {
    if (!textureDiffuse1Mat.isActive) return vec3(0.0, 0.0, 0.0);
    return vec3(texture(textureDiffuse1, coords));
}

vec3 getSpecTexturePixel(vec2 coords) {
    if (!textureSpecular1Mat.isActive) return vec3(0.0, 0.0, 0.0);
    return vec3(texture(textureSpecular1, coords));
}

const float AMBIENT_LIGHT = 0.5;

vec3 calcAmbientLight(vec2 coords) {
    return texture(textureDiffuse1, coords).rgb * AMBIENT_LIGHT;
}

vec3 calcDiffuseLight(uint lightId, vec2 coords) {
    vec3 Diffuse = texture(textureDiffuse1, coords).rgb;
    vec3 lightDir = normalize(lights[lightId].position - FragPos);
    return max(dot(Normal, lightDir), 0.0) * Diffuse * lights[lightId].color;
}

vec3 calcSpecularLight(uint lightId, vec2 coords) {
    float Specular = texture(textureSpecular1, coords).r;
    vec3 viewDir  = normalize(ViewPos - FragPos);
    vec3 lightDir = normalize(lights[lightId].position - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0); // shinessness 
            // will go here
    vec3 specular = lights[lightId].color * spec * Specular;
    return specular;
}

float calcAttenuation(uint lightId, float distToLightSource) {
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

    float viewZ = -ViewSpacePos.z;
    uint tileZ = uint((log(viewZ / zNear) * gridSize.z) / log(zFar / zNear));
    tileZ = clamp(tileZ, 0u, gridSize.z - 1u);

    return tileXY.x + tileXY.y * gridSize.x + tileZ * gridSize.x * gridSize.y;
}

void main() {
    uint clusterIndex = findClusterIndex();
    Cluster currCluster = clusters[clusterIndex];
    
    // vec3 result = calcAmbientLight(TexCoords);
    // for (int i = 0; i < currCluster.count; ++i) {
    //     uint lightIndex = currCluster.lightIndices[i];

    //     float dist = length(lights[lightIndex].position - FragPos);

    //     if (dist < lights[lightIndex].radius) {
    //         vec3 diffuse = calcDiffuseLight(lightIndex, TexCoords);
    //         vec3 specular = calcSpecularLight(lightIndex, TexCoords);
    //         float attenuation = calcAttenuation(lightIndex, dist);

    //         result += (diffuse + specular) * attenuation;
    //     }
    // }

    // FragColor = vec4(result, 1.0); 

    // if (currCluster.count > 10) {
    //     FragColor = vec4(1.0, 0.0, 0.0, 1.0);    // Red: many lights
    // } else if (currCluster.count > 0) {
    //     FragColor = vec4(1.0, 1.0, 0.0, 1.0);    // Yellow: some lights  
    // } else {
    //     FragColor = vec4(0.0, 0.0, 1.0, 1.0);    // Blue: no lights
    // }

    if (clusters[clusterIndex].count == 888) {
        FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Green: cull shader ran
    } else if (clusters[clusterIndex].count == 999) {
        FragColor = vec4(1.0, 1.0, 0.0, 1.0); // Yellow: build shader ran
    } else if (clusters[clusterIndex].count == 0) {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red: no data written

        float viewZ = -ViewSpacePos.z;
        if (viewZ > zFar) {
            FragColor = vec4(1.0, 1.0, 0.0, 1.0); // Yellow: beyond far plane
        } else if (viewZ < zNear) {
            FragColor = vec4(1.0, 0.0, 1.0, 1.0); // Magenta: too close
        }
    } else if (clusters[clusterIndex].count > 1000) {
        FragColor = vec4(1.0, 0.0, 1.0, 1.0); 
    } else {
        FragColor = vec4(0.0, 0.0, 1.0, 1.0); // Blue: some other value
    }

    // uint clusterThird = clusterIndex / (3456 / 3);
    
    // if (clusterThird == 0) {
    //     FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red: first third of clusters
    // } else if (clusterThird == 1) {
    //     FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Green: second third
    // } else {
    //     FragColor = vec4(0.0, 0.0, 1.0, 1.0); // Blue: last third
    // }
}
