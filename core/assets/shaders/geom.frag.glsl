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
    gNormal = normalize(v_Normal);

    gAlbedoSpec.rgb = texture(get_diffuseMap(), v_UV).rgb;
    gAlbedoSpec.a = texture(get_specularMap(), v_UV).r;
    // gAlbedoSpec = vec4(0.5, 0.5, 0.5, 1.0);;
}
