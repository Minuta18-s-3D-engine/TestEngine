//@section("GVertex")

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

//@section("GFragment")

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

//@section("LFragment")

//@import("core://shaders/lightingPassStdLib.glsl")

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
// Uniforms:
// u_Time              - (float) Time in seconds.
// u_DeltaTime         - (float) Frame time.
// u_Frame             - (uint) Index of current frame.
// u_Resolution        - (ivec2) Screen resolution.
// u_TexelSize         - (vec2) 1 / u_Resolution.
// u_CameraPosition    - (vec3) Camera position vector.
// u_CameraDirection   - (vec3) Camera direction vector.
// u_View              - (mat4) World-to-View matrix.
// u_Projection        - (mat4) View-to-Clip matrix.
// u_InvView, u_InvProjection (mat4) inverse matrices.
// u_ZNear, u_ZFar     - (float) Camera clipping planes.
// Clustered shading data:
// u_GridSize          - (uvec3) Cluster grid dimensions.
// b_Clusters          - (SSBO[Cluster]) SSBO of clusters.
// b_PointLights       - (SSBO[PointLight]) SSBO of point lights. 
// b_LightIndices      - (SSBO[uint]) SSBO of point light's clusters indices.
// BVH data:
// b_BVHNodes          - (SSBO[BVHNode]) SSBO of bvh nodes.
// b_BVHIndices        - (SSBO[uint]) SSBO of bvh leaf indices.
//
// Built-in structures available in the shader:
// Cluster:
// .minPoint            - (vec4) AABB minimum
// .maxPoint            - (vec4) AABB maximum
// .count               - (uint) Number of lights in a cluster
// .lightStart          - (uint) Offset in b_LightIndices buffer
// PointLight:
// .position            - (vec3) Light position
// .color               - (vec3) Light color
// .linear, quadratic   - (uint) Coefficients fade
// .radius              - (uint) Radius of area affected by light, calculated 
//                       using linear and quadratic
// BVHNode:
// .minPoint            - (vec4) AABB minimum
// .maxPoint            - (vec4) AABB maximum
// .first_child_or_primitive - (uint) Index of first child or primitive
// .primitive_count     - (uint) primitive count

vec3 worldPos = texture(s_GPosition, v_TexCoords).rgb;
vec3 worldNormal = texture(s_GNormal, v_TexCoords).rgb;
vec4 viewSpacePos = u_View * vec4(worldPos, 1.0);
vec4 albedoSpec = texture(s_GAlbedo, v_TexCoords);

void lighting() {
    // Your code here
}
