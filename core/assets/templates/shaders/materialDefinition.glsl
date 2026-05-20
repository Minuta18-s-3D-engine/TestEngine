struct Material_Structure_{{ material_name }} {
{{ material_properties }}
};

{{ sampler_definitions }}

{{ sampler_getters }}

Material_Structure_{{ material_name }} currentMaterial;

void loadCurrentMaterial() {
    uint base = u_CurrentMaterialStartId;
    {{ unpack_lines }}   
}