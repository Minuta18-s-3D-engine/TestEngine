#define MATH_EPS 0.00001
#define LIGHTS_PRE_CLUSTER_LIMIT 256

uniform uvec3 u_GridSize;

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

vec3 screenToView(vec2 screenCoord) {
    vec4 ndc = vec4(screenCoord / u_Resolution * 2.0 - 1.0, -1.0, 1.0);

    vec4 viewCoord = u_InvProjection * ndc;
    viewCoord /= viewCoord.w;
    return viewCoord.xyz;
}

vec3 lineIntersectionWithZPlane(
    vec3 startPoint, 
    vec3 endPoint, 
    float zDistance
) {
    vec3 direction = endPoint - startPoint;
    vec3 normal = vec3(0.0, 0.0, -1.0);

    float t = (zDistance - dot(normal, startPoint)) / dot(normal, direction);
    return startPoint + t * direction;
}

void compute() {
    uint tileIndex = gl_WorkGroupID.x + (gl_WorkGroupID.y * u_GridSize.x) +
            (gl_WorkGroupID.z * u_GridSize.x * u_GridSize.y);   
    vec2 tileSize = vec2(u_Resolution) / vec2(u_GridSize.xy);

    vec2 minTile_screenspace = gl_WorkGroupID.xy * tileSize;
    vec2 maxTile_screenspace = (gl_WorkGroupID.xy + 1) * tileSize;

    vec3 minTile = screenToView(minTile_screenspace);
    vec3 maxTile = screenToView(maxTile_screenspace);

    // Exponential formula by DaveH355
    // https://github.com/DaveH355/clustered-shading
    float planeNear = u_ZNear * pow(u_ZFar / u_ZNear, 
        gl_WorkGroupID.z / float(u_GridSize.z));
    float planeFar = u_ZNear * pow(u_ZFar / u_ZNear, (gl_WorkGroupID.z + 1) / 
        float(u_GridSize.z));

    // Points calculation
    vec3 minPointNear = lineIntersectionWithZPlane(
        vec3(0, 0, 0), minTile, planeNear);
    vec3 minPointFar = lineIntersectionWithZPlane(
        vec3(0, 0, 0), minTile, planeFar);
    vec3 maxPointNear = lineIntersectionWithZPlane(
        vec3(0, 0, 0), maxTile, planeNear);
    vec3 maxPointFar = lineIntersectionWithZPlane(
        vec3(0, 0, 0), maxTile, planeFar);

    b_Clusters[tileIndex].minPoint = vec4(min(minPointNear, minPointFar), 0.0);
    b_Clusters[tileIndex].maxPoint = vec4(max(maxPointNear, maxPointFar), 0.0);
    b_Clusters[tileIndex].count = 0; 
    b_Clusters[tileIndex].lightStart = LIGHTS_PRE_CLUSTER_LIMIT * tileIndex;
}
