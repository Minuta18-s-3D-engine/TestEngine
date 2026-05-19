layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 v_WorldPos;
in vec2 v_UV;
in vec3 v_Normal;
in vec3 v_Tangent;
in vec3 v_Bitangent;