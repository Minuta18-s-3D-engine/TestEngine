#version 430 core

#define LOCAL_SIZE 128
#define LIGHTS_PRE_CLUSTER_LIMIT 256
#define BVH_STACK_SIZE 64

layout(local_size_x = LOCAL_SIZE, local_size_y = 1, local_size_z = 1) in;

struct PointLight {
    vec3 position;
    vec3 color;
    
    float linear;
    float quadratic;
    float radius;
};

struct Cluster {
    vec4 minPoint;
    vec4 maxPoint;
    uint count;
    uint lightStart;
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

uniform mat4 viewMat;
uniform uint numLights;
uniform uvec3 gridSize;
uniform int currentDispatch;

bool sphereAABBIntersection(vec3, float, vec3, vec3);
bool testSphereAABB(uint, uint);

bool sphereAABBIntersection(
    vec3 center, 
    float radius, 
    vec3 aabbMin, 
    vec3 aabbMax
) {
    vec3 closestPoint = clamp(center, aabbMin, aabbMax);

    float distanceSquared = dot(closestPoint - center, closestPoint - center);
    return (distanceSquared <= (radius * radius));
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

bool testSphereAABB(uint lightInd, Cluster currCluster) {
    vec3 center = vec3(viewMat * vec4(lights[lightInd].position, 1.0));
    float radius = lights[lightInd].radius;

    vec3 aabbMin = currCluster.minPoint.xyz;
    vec3 aabbMax = currCluster.maxPoint.xyz;

    return sphereAABBIntersection(center, radius, aabbMin, aabbMax);
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
        vec3 viewCorner = vec3(viewMat * vec4(worldCorners[i], 1.0));
        viewMin.x = min(viewMin.x, viewCorner.x);
        viewMin.y = min(viewMin.y, viewCorner.y);
        viewMin.z = min(viewMin.z, viewCorner.z);
        viewMax.x = max(viewMax.x, viewCorner.x);
        viewMax.y = max(viewMax.y, viewCorner.y);
        viewMax.z = max(viewMax.z, viewCorner.z);
    }
    
    BVHNode result;
    result.minPoint = vec4(viewMin, 0.0);
    result.maxPoint = vec4(viewMax, 0.0);
    result.first_child_or_primitive = node.first_child_or_primitive;
    result.primitive_count = node.primitive_count;
    
    return result;
}

void main() {
    uint clusterInd = gl_WorkGroupID.x * LOCAL_SIZE + gl_LocalInvocationID.x;

    uint totalClusters = gridSize.x * gridSize.y * gridSize.z;
    if (clusterInd >= totalClusters) {
        return;
    }

    Cluster currCluster = clusters[clusterInd];

    currCluster.count = 0;

    uint stack[BVH_STACK_SIZE];
    uint stackPtr = 0;
    stack[stackPtr++] = 0;

    uint leavesFound = 0;

    while (stackPtr > 0 && currCluster.count < LIGHTS_PRE_CLUSTER_LIMIT) {
        uint nodeIndex = stack[--stackPtr];
        BVHNode node = bvhNodes[nodeIndex];
        node = transformBVHNodeToViewSpace(node);

        if (!AABBIntersection(
            currCluster.maxPoint.xyz, currCluster.minPoint.xyz, 
            node.maxPoint.xyz, node.minPoint.xyz
        )) {
            continue;
        }

        if (node.primitive_count > 0) {
            for (uint i = 0; i < node.primitive_count && currCluster.count < LIGHTS_PRE_CLUSTER_LIMIT; ++i) {
                uint lightIndex = bvhIndices[node.first_child_or_primitive + i];
                if (testSphereAABB(lightIndex, currCluster)) {
                    pointLightIndicies[currCluster.lightStart + currCluster.count] = lightIndex;
                    currCluster.count++;
                }
            }
        } else {
            if (stackPtr < BVH_STACK_SIZE - 2) {
                uint firstChild = node.first_child_or_primitive;
                stack[stackPtr++] = firstChild;
                stack[stackPtr++] = firstChild + 1;
            }
        }
    }
    
    // currCluster.count = nodesSkipped;
    clusters[clusterInd] = currCluster;
}
