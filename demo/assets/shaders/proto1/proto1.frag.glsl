layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 v_WorldPos;
in vec2 v_UV;
in vec3 v_Normal;
in vec3 v_Tangent;
in vec3 v_Bitangent;

void fragment() {
    gPosition = v_WorldPos;
    vec3 normal = normalize(v_Normal);
    gNormal = normalize(normal);

    vec3 scaledPos = v_WorldPos * currentMaterial.tilingScale;
    float patternX = mod(floor(scaledPos.y) + floor(scaledPos.z), 2.0);
    float patternY = mod(floor(scaledPos.x) + floor(scaledPos.z), 2.0);
    float patternZ = mod(floor(scaledPos.x) + floor(scaledPos.y), 2.0);

    vec3 blendWeights = abs(normal);
    blendWeights /= (blendWeights.x + blendWeights.y + blendWeights.z);

    float pattern = patternX * blendWeights.x + 
        patternY * blendWeights.y + 
        patternZ * blendWeights.z;
    
    pattern = step(0.5, pattern);

    vec3 baseColor = currentMaterial.baseColor;
    vec3 darkColor = baseColor - baseColor * 0.2;

    gAlbedoSpec.rgb = mix(darkColor, baseColor, pattern);
    gAlbedoSpec.a = 0.5;
}
