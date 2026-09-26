struct __Generated_ShaderParams {
{{ shader_properties }}
};

__Generated_ShaderParams shaderParams;

{{ sampler_definitions }}

{{ sampler_getters }}

void __Generated_loadShaderParams() {
    uint base = u_CurrentMaterialStartId;

    {{ unpack_lines }}
}
