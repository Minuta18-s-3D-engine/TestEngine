layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;

out vec2 v_UV;

void vertex() {
    v_UV = a_TexCoords;
    gl_Position = vec4(a_Position, 1.0);
}
