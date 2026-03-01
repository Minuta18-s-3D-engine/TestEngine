uint findClusterIndex() {
    vec3 worldPos = texture(s_GPosition, v_TexCoords).rgb;
    vec4 viewSpacePos = u_View * vec4(worldPos, 1.0);

    uvec2 tileSize = u_Resolution / u_GridSize.xy;
    uvec2 tileXY = uvec2(gl_FragCoord.xy) / tileSize;
    tileXY = min(tileXY, u_GridSize.xy - 1u);

    uint tileZ = uint((log(abs(viewSpacePos.z) / u_ZNear) * u_GridSize.z) / 
        log(u_ZFar / u_ZNear));
    tileZ = clamp(tileZ, 0u, u_GridSize.z - 1u);

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

bool AABBIntersection(vec3 aMax, vec3 aMin, vec3 bMax, vec3 bMin) {
    return (aMin.x <= bMax.x) && (aMax.x >= bMin.x) &&
           (aMin.y <= bMax.y) && (aMax.y >= bMin.y) &&
           (aMin.z <= bMax.z) && (aMax.z >= bMin.z);
}