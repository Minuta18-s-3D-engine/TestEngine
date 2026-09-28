struct __Generated_ShaderParams {
{{ shader_properties }}
};

__Generated_ShaderParams shaderParams;

{{ sampler_uniforms }}

{{ sampler_getters }}

void __Generated_loadShaderParams() {
    uint base = u_CurrentMaterialStartId;

    {{ unpack_lines }}
}

/*
int:     int(b_MaterialData[base + 0])
uint:    (b_MaterialData[base + 0])
int64:   packInt2x32(ivec2(b_MaterialData[base + 0], b_MaterialData[base + 1]))
uint64:  packUint2x32(ivec2(b_MaterialData[base + 0], b_MaterialData[base + 1]))
float:   uintBitsToFloat(b_MaterialData[base + 0])
bool:    (b_MaterialData[base + 0] != 0U)

ivec2:   ivec2(int(b_MaterialData[base + 0]), int(b_MaterialData[base + 1]))
vec3:    vec3(uintBitsToFloat(b_MaterialData[base + 0]), uintBitsToFloat(b_MaterialData[base + 1]), uintBitsToFloat(b_MaterialData[base + 2]))


*/