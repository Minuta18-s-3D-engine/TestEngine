{{ material_definition }}

// Built-in variables available in the shader:
// currentMaterial     - ({{ material_name }}) variable that contains current 
//                       material.
// Attributes:
// a_Position          - (vec3) Vertex position in Object Space.
// a_TexCoords         - (vec2) UV coordinates.
// a_Normal            - (vec3) Normal in Object Space.
// a_Tangent           - (vec3) Tangent for normal mapping.
// a_Bitangent         - (vec3) Bitangent for normal mapping.
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
// Outputs:
// v_Data.worldPos     - (vec3) World-space position.
// v_Data.uv           - (vec2) UV coordinates.
// v_Data.normal       - (vec3) World-space normal.

void vertex() {
    // Your code here

    // Example: Simple transform:
    // gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}
