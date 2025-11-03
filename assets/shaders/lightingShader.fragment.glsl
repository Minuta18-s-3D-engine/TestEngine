#version 430 core
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

in vec2 TexCoords;
uniform vec3 viewPos;
uniform uint drawMode;

uniform float zNear;
uniform float zFar;
uniform uvec3 gridSize;
uniform uvec2 screenDimensions;
uniform mat4 projection;
uniform mat4 view;

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
vec4 ViewSpacePos = view * vec4(FragPos, 1.0);
vec3 Normal = texture(gNormal, TexCoords).rgb;

vec3 calcPointLight(PointLight pLight) {
    vec3 pathToLight = pLight.position - FragPos;
    float distSq = dot(pathToLight, pathToLight);
    float radiusSq = pLight.radius * pLight.radius;

    if (distSq > radiusSq) return vec3(0.0);

    vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(pLight.position - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); 

    vec3 albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float specular = texture(gAlbedoSpec, TexCoords).a;;

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

uint findClusterIndex() {
    uvec2 tileSize = screenDimensions / gridSize.xy;
    uvec2 tileXY = uvec2(gl_FragCoord.xy) / tileSize;
    tileXY = min(tileXY, gridSize.xy - 1u);

    uint tileZ = uint((log(abs(ViewSpacePos.z) / zNear) * gridSize.z) / log(zFar / zNear));
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

    vec3 result = calcAmbientLight(TexCoords);
    uint lightCount = currCluster.count;

    for (int i = 0; i < lightCount; ++i) {
        uint lightIndex = pointLightIndicies[currCluster.lightStart + i];
        PointLight l = lights[lightIndex]; 
        
        result += calcPointLight(l);
    }

    if (drawMode == 0) {
        FragColor = vec4(result, 1.0);
    } else if (drawMode == 1) {
        float cnt = currCluster.count;
        FragColor = vec4(cnt / 100.0, cnt / 100.0, cnt / 100.0, 1.0);
    } else {

        uint zTile = uint((log(abs(ViewSpacePos.z) / zNear) * gridSize.z) / log(zFar / zNear));

        if (zTile % 5 == 0) {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        } else if (zTile % 5 == 1) {
            FragColor = vec4(0.0, 1.0, 0.0, 1.0);
        } else if (zTile % 5 == 2) {
            FragColor = vec4(0.0, 0.0, 1.0, 1.0);
        } else if (zTile % 5 == 3) {
            FragColor = vec4(1.0, 0.0, 1.0, 1.0);
        } else {
            FragColor = vec4(1.0, 1.0, 0.0, 1.0);
        }

        if (clusterIndex >= totalClusters || clusterIndex < 0) {
            FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        }
    }
}
