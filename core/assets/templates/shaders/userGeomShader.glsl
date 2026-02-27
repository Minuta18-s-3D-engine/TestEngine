//@section("vertex")

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
// u_Model             - (mat4) Model-to-World matrix.
// u_View              - (mat4) World-to-View matrix.
// u_Projection        - (mat4) View-to-Clip matrix.
// Outputs:
// v_Data.worldPos     - (vec3) World-space position.
// v_Data.uv           - (vec2) UV coordinates.
// v_Data.normal       - (vec3) World-space normal.

void vertex() {
    // Your code here

    // Example: Simple transform:
    // gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

//@section("fragment")

{{ material_definition }}

// Built-in variables available in the shader:
// currentMaterial     - ({{ material_name }}) variable that contains current 
//                       material.
// Inputs:
// v_Data              - (struct) Interpolated data from vertex shader.
// Outputs (G-Buffer):
// out_Position        - (vec3) World-space position.
// out_Normal          - (vec3) World-space normal.
// out_Albedo          - (vec4) Albedo color.

void fragment() {
    // Your code here

    // Example: Setting Albedo from diffuse texture:
    // out_Albedo = texture(s_Diffuse, v_Data.uv);
}

//@section("lighting_pass")

{{ material_definition }}

// Built-in variables available in the shader:
// currentMaterial     - ({{ material_name }}) variable that contains current 
//                       material
// G-Buffer:
// s_GPosition         - (sampler2D) G-Buffer texture, which contains pixels' 
//                       world position.
// s_GNormal           - (sampler2D) G-Buffer texture, which contains normal.
// s_GAlbedoSpec       - (sampler2D) G-Buffer texture, which contains pixel's
//                       albedo (.xyz) and specular (.w) value.
// v_TexCoords         - (vec2) Fullscreen quad UVs.
// Camera and screen params:
// u_ViewPos           - (vec3) Camera position in world space.
// u_ScreenResolution  - (uvec2) Screen resolution.
// u_ZNear, u_ZFar     - (float) Camera clipping planes.
// Clustered shading data:
// u_GridSize          - (uvec3) Cluster grid dimensions.
// b_Clusters          - (SSBO[Cluster]) SSBO of clusters.
// b_PointLights       - (SSBO[PointLight]) SSBO of point lights. 
// b_LightIndices      - (SSBO[uint]) SSBO of point light's clusters indices.
// BVH data:
// b_BVHNodes          - (SSBO[BVHNode]) SSBO of bvh nodes.
// b_BVHIndices        - (SSBO[uint]) SSBO of bvh leaf indices.

vec3 worldPos = texture(s_GPosition, v_TexCoords).rgb;
vec3 worldNormal = texture(s_GNormal, v_TexCoords).rgb;
vec4 viewSpacePos = u_View * vec4(worldPos, 1.0);
vec4 albedoSpec = texture(s_GAlbedo, v_TexCoords);

void lighting() {
    // Your code here
}
