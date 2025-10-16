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

layout (std430, binding = 0) restrict coherent buffer clusterSSBO {
    Cluster clusters[];
};

layout (std430, binding = 1) restrict coherent buffer lightSSBO {
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

    uint totalClusters = gridSize.x * gridSize.y * gridSize.z;
    if (clusterIndex >= totalClusters) {
        return;
    }

    if (currCluster.count > 0) {
        FragColor = vec4(lights[0].color.r, lights[0].color.g, lights[0].color.b, 1.0);
    }
}
