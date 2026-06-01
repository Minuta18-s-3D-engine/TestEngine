struct Material_Structure_{{ material_name }} {
{{ material_properties }}
};

{{ sampler_definitions }}

Material_Structure_{{ material_name }} currentMaterial;

{{ sampler_getters }}

void loadCurrentMaterial() {
    uint base = u_CurrentMaterialStartId;
{{ unpack_lines }}   
}