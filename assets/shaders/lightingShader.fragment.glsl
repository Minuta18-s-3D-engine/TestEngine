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

struct BVHNode {
    vec4 minPoint;
    vec4 maxPoint;
    uint first_child_or_primitive;
    uint primitive_count;
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

layout(std430, binding = 3) restrict buffer bvhNodesSSBO {
    BVHNode bvhNodes[];
};

layout(std430, binding = 4) restrict buffer bvhIndicesSSBO {
    uint bvhIndices[];
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

BVHNode transformBVHNodeToViewSpace(BVHNode node) {
    vec3 worldCorners[8] = {
        node.minPoint.xyz,
        vec3(node.minPoint.x, node.minPoint.y, node.maxPoint.z),
        vec3(node.minPoint.x, node.maxPoint.y, node.minPoint.z),
        vec3(node.minPoint.x, node.maxPoint.y, node.maxPoint.z),
        vec3(node.maxPoint.x, node.minPoint.y, node.minPoint.z),
        vec3(node.maxPoint.x, node.minPoint.y, node.maxPoint.z),
        vec3(node.maxPoint.x, node.maxPoint.y, node.minPoint.z),
        node.maxPoint.xyz
    };
    
    vec3 viewMin = vec3(1e9);
    vec3 viewMax = vec3(-1e9);
    
    for (int i = 0; i < 8; i++) {
        vec3 viewCorner = vec3(view * vec4(worldCorners[i], 1.0));
        viewMin = min(viewMin, viewCorner);
        viewMax = max(viewMax, viewCorner);
    }
    
    BVHNode result;
    result.minPoint = vec4(viewMin, 0.0);
    result.maxPoint = vec4(viewMax, 0.0);
    result.first_child_or_primitive = node.first_child_or_primitive;
    result.primitive_count = node.primitive_count;
    
    return result;
}

bool AABBIntersection(
    vec3 aMax,
    vec3 aMin,
    vec3 bMax,
    vec3 bMin
) {
    return (aMin.x <= bMax.x) && (aMax.x >= bMin.x) &&
           (aMin.y <= bMax.y) && (aMax.y >= bMin.y) &&
           (aMin.z <= bMax.z) && (aMax.z >= bMin.z);
}

bool isCenterCross() {
    return (distance(gl_FragCoord.xy, screenDimensions.xy / 2) < 2);
}

void main() {
    int modifier = 1;
    if (isCenterCross()) {
        modifier = -1;
    }

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
        if (modifier == -1) {
            FragColor = vec4(vec3(1.0) - result, 1.0);
            return;
        }

        FragColor = vec4(result, 1.0);

    } else if (drawMode == 1) {
        float cnt = currCluster.count;
        FragColor = vec4(cnt / 100.0, cnt / 100.0, cnt / 100.0, 1.0);
    } else {
        BVHNode testNode = bvhNodes[1]; 
        testNode = transformBVHNodeToViewSpace(testNode);
        
        bool intersects = AABBIntersection(
            currCluster.maxPoint.xyz, currCluster.minPoint.xyz,
            testNode.maxPoint.xyz, testNode.minPoint.xyz        
        );
        
        if (intersects) {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0); 
        } else {
            FragColor = vec4(0.0, 0.0, 1.0, 1.0); 
        }
        return;
    }
}
