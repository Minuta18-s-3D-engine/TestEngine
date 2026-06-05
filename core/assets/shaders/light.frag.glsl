out vec4 FragColor;
in vec2 v_UV;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedoSpec;

uniform uvec3 u_GridSize;
uniform uint u_DrawMode;

vec3 FragPos = texture(gPosition, v_UV).rgb;
vec4 ViewSpacePos = u_View * vec4(FragPos, 1.0);
vec3 Normal = texture(gNormal, v_UV).rgb;

vec3 calcPointLight(PointLight pLight) {
    vec3 pathToLight = pLight.position - FragPos;
    float distSq = dot(pathToLight, pathToLight);
    float radiusSq = pLight.radius * pLight.radius;

    if (distSq > radiusSq) return vec3(0.0);

    vec3 viewDir  = normalize(u_CameraPosition - FragPos);
    vec3 lightDir = normalize(pLight.position - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); 

    vec3 albedo = texture(gAlbedoSpec, v_UV).rgb;
    float specular = texture(gAlbedoSpec, v_UV).a;;

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
    uvec2 tileSize = u_Resolution / u_GridSize.xy;
    uvec2 tileXY = uvec2(gl_FragCoord.xy) / tileSize;
    tileXY = min(tileXY, u_GridSize.xy - 1u);

    uint tileZ = uint((log(abs(ViewSpacePos.z) / u_ZNear) * u_GridSize.z) / log(u_ZFar / u_ZNear));
    tileZ = clamp(tileZ, 0u, u_GridSize.z - 1u);

    return tileXY.x + tileXY.y * u_GridSize.x + tileZ * u_GridSize.x * u_GridSize.y;
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
        vec3 viewCorner = vec3(u_View * vec4(worldCorners[i], 1.0));
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
    return (distance(gl_FragCoord.xy, u_Resolution.xy / 2) < 2);
}

void fragment() {

    // FragColor = vec4(texture(gAlbedoSpec, v_UV).xyz, 1.0);
    // return;

    int modifier = 1;
    if (isCenterCross()) {
        modifier = -1;
    }

    uint clusterIndex = findClusterIndex();
    Cluster currCluster = b_Clusters[clusterIndex];

    uint totalClusters = u_GridSize.x * u_GridSize.y * u_GridSize.z;
    if (clusterIndex >= totalClusters) {
        return;
    }

    vec3 result = calcAmbientLight(v_UV);
    uint lightCount = currCluster.count;

    for (int i = 0; i < lightCount; ++i) {
        uint lightIndex = b_PointLightIndicies[currCluster.lightStart + i];
        PointLight l = b_Lights[lightIndex]; 
        
        result += calcPointLight(l);
    }

    if (u_DrawMode == 0) {
        if (modifier == -1) {
            FragColor = vec4(vec3(
                1.0 - result.x,
                1.0 - result.y,
                1.0 - result.z
            ), 1.0);
            return;
        }

        FragColor = vec4(result, 1.0);

    } else if (u_DrawMode == 1) {
        float cnt = currCluster.count;
        FragColor = vec4(cnt / 100.0, cnt / 100.0, cnt / 100.0, 1.0);
    } else {
        BVHNode testNode = b_BvhNodes[1]; 
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
