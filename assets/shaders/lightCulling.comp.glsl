#version 430 core

#define LOCAL_SIZE 128
#define LIGHTS_PRE_CLUSTER_LIMIT 100

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
    uint lightIndices[100];
};

layout (std430, binding = 0) restrict buffer clusterSSBO {
    Cluster clusters[];
};

layout (std430, binding = 1) restrict buffer lightSSBO {
    PointLight lights[];
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
    vec3 closestPoint = max(aabbMax, min(center, aabbMin));

    float distanceSquared = dot(closestPoint - center, closestPoint - center);
    return distanceSquared <= (radius * radius);
}

bool testSphereAABB(uint lightInd, Cluster currCluster) {
    vec4 lightViewPos = viewMat * vec4(lights[lightInd].position, 1.0);
    vec3 center = lightViewPos.xyz;
    float radius = lights[lightInd].radius;

    vec3 aabbMin = currCluster.minPoint.xyz;
    vec3 aabbMax = currCluster.maxPoint.xyz;

    return sphereAABBIntersection(center, radius, aabbMin, aabbMax);
}

void main() {
    uint clusterInd = gl_WorkGroupID.x * LOCAL_SIZE + gl_LocalInvocationID.x;

    uint totalClusters = gridSize.x * gridSize.y * gridSize.z;
    if (clusterInd >= totalClusters) {
        return;
    }

    Cluster currCluster = clusters[clusterInd];

    currCluster.count = 0;
    for (uint i = 0; i < numLights; ++i) {
        if (testSphereAABB(i, currCluster) && 
            currCluster.count < LIGHTS_PRE_CLUSTER_LIMIT) {
            currCluster.lightIndices[currCluster.count] = i;
            currCluster.count++;
        }
    }

    // currCluster.lightIndices[0] = 0;
    // currCluster.count = 1;
    
    // clusters[clusterInd] = currCluster;

    // if (clusterInd < 100) {
    //     clusters[clusterInd].count = 1;
    //     clusters[clusterInd].lightIndices[0] = 0;
    // }

    clusters[clusterInd].count = 888;
    clusters[clusterInd].lightIndices[0] = 0;

    // if (clusterInd >= gridSize.x * gridSize.y * gridSize.z) {
    //     clusters[clusterInd].count = 9999; // Out of bounds - yellow
    // }
}
