{{ material_definition }}

// Built-in variables available in the shader:
// currentMaterial     - ({{ material_name }}) variable that contains current 
//                       material.
// Inputs:
// v_Data              - (struct) Interpolated data from vertex shader.
// Uniforms:
// u_Time              - (float) Time in seconds.
// u_DeltaTime         - (float) Frame time.
// u_Frame             - (uint) Index of current frame.
// u_Resolution        - (ivec2) Screen resolution.
// u_TexelSize         - (vec2) 1 / u_Resolution.
// u_CameraPosition    - (vec3) Camera position vector.
// u_CameraDirection   - (vec3) Camera direction vector.
// u_Model             - (mat4) Model-to-World matrix.
// u_View              - (mat4) World-to-View matrix.
// u_Projection        - (mat4) View-to-Clip matrix.
// u_InvModel, u_InvView, u_InvProjection (mat4) inverse matrices.
// Outputs (G-Buffer):
// out_Position        - (vec3) World-space position.
// out_Normal          - (vec3) World-space normal.
// out_Albedo          - (vec4) Albedo color.

void fragment() {
    // Your code here

    // Example: Setting Albedo from diffuse texture:
    // out_Albedo = texture(s_Diffuse, v_Data.uv);
}
