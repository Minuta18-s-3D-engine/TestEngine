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