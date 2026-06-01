layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Bitangent;

out vec3 v_WorldPos;
out vec2 v_UV;
out vec3 v_Normal;
out vec3 v_Tangent;
out vec3 v_Bitangent;

void vertex() {
    vec4 worldPos = u_Model * vec4(a_Position, 1.0);
    v_WorldPos = worldPos.xyz;

    v_UV = a_TexCoords;

    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    v_Normal = normalMatrix * a_Normal;

    gl_Position = u_Projection * u_View * worldPos;
}
