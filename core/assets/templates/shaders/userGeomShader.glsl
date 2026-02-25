//@preprocessor.section("vertex")

{{ material_definition }}

// Built-in variables available in the shader:
// currentMaterial     - ({{ material_name }}) variable that contains current 
//                       material
// Attributes:
// a_Position          - (vec3) Vertex position in Object Space
// a_TexCoords         - (vec2) UV coordinates
// a_Normal            - (vec3) Normal in Object Space
// a_Tangent           - (vec3) Tangent for normal mapping
// a_Bitangent         - (vec3) Bitangent for normal mapping
// Uniforms:
// u_Model             - (mat4) Model-to-World matrix
// u_View              - (mat4) World-to-View matrix
// u_Projection        - (mat4) View-to-Clip matrix
// Outputs:
// v_Data.worldPos     - (vec3) World-space position
// v_Data.uv           - (vec2) UV coordinates
// v_Data.normal       - (vec3) World-space normal

void vertex() {
    // Your code here

    // Example: Simple transform:
    // gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

// @preprocessor.section("fragment")

{{ material_definition }}

// Built-in variables available in the shader:
// Inputs:
// v_Data              - (struct) Interpolated data from vertex shader
// Texture samplers:
// s_Diffuse           - (sampler2D) Diffuse texture
// s_Specular          - (sampler2D) Specular texture
// s_Normal            - (sampler2D) Normal map texture
// Outputs (G-Buffer):
// out_Position        - (vec3) World-space position
// out_Normal          - (vec3) World-space normal
// out_Albedo          - (vec4) Albedo color

void fragment() {
    // Example: Setting Albedo from diffuse texture:
    // out_Albedo = texture(s_Diffuse, v_Data.uv);
}
