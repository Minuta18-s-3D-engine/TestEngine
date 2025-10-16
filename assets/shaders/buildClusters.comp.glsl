#version 430 core

#define MATH_EPS 0.00001
#define LIGHTS_PRE_CLUSTER_LIMIT 100

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

struct Cluster {
    vec4 minPoint;
    vec4 maxPoint;
    uint count;
    uint lightIndices[LIGHTS_PRE_CLUSTER_LIMIT];
};

layout(std430, binding = 0) restrict coherent buffer clusterSSBO {
    Cluster clusters[];
};

uniform float zNear;
uniform float zFar;
uniform mat4 inverseProjection;
uniform uvec3 gridSize;
uniform uvec2 screenDimensions;
uniform int currentDispatch;

vec3 screenToView(vec2 screenCoord);
vec3 lineIntersectionWithZPlane(
    vec3 startPoint, 
    vec3 endPoint, 
    float zDistance
);

vec3 screenToView(vec2 screenCoord) {
    vec4 ndc = vec4(screenCoord / screenDimensions * 2.0 - 1.0, -1.0, 1.0);

    vec4 viewCoord = inverseProjection * ndc;
    viewCoord /= viewCoord.w;
    return viewCoord.xyz;
}

vec3 lineIntersectionWithZPlane(
    vec3 startPoint, 
    vec3 endPoint, 
    float zDistance
) {
    vec3 direction = endPoint - startPoint;

    if (abs(direction.z) < MATH_EPS) {
        return startPoint;
    }

    float t = (zDistance - startPoint.z) / direction.z;
    return startPoint + t * direction;
}

void main() {
    uint tileIndex = gl_WorkGroupID.x + (gl_WorkGroupID.y * gridSize.x) +
            (gl_WorkGroupID.z * gridSize.x * gridSize.y);
    
    vec2 tileSize = vec2(screenDimensions) / vec2(gridSize.xy);

    vec2 minTile_screenspace = gl_WorkGroupID.xy * tileSize;
    vec2 maxTile_screenspace = (gl_WorkGroupID.xy + 1) * tileSize;

    vec3 minTile = screenToView(minTile_screenspace);
    vec3 maxTile = screenToView(maxTile_screenspace);

    // Exponential formula by DaveH355
    // https://github.com/DaveH355/clustered-shading
    float planeNear = zNear * pow(zFar / zNear, 
        gl_WorkGroupID.z / float(gridSize.z));
    float planeFar = zNear * pow(zFar / zNear, (gl_WorkGroupID.z + 1) / 
        float(gridSize.z));

    // Points calculation
    vec3 minPointNear = lineIntersectionWithZPlane(
        vec3(0, 0, 0), minTile, planeNear);
    vec3 minPointFar = lineIntersectionWithZPlane(
        vec3(0, 0, 0), minTile, planeFar);
    vec3 maxPointNear = lineIntersectionWithZPlane(
        vec3(0, 0, 0), maxTile, planeNear);
    vec3 maxPointFar = lineIntersectionWithZPlane(
        vec3(0, 0, 0), maxTile, planeFar);

    clusters[tileIndex].minPoint = vec4(min(minPointNear, minPointFar), 0.0);
    clusters[tileIndex].maxPoint = vec4(max(maxPointNear, maxPointFar), 0.0);
    clusters[tileIndex].count = 0; 
}
