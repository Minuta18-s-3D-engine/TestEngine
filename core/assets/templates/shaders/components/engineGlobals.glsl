struct Cluster {
    vec4 minPoint;
    vec4 maxPoint;
    uint count;
    uint lightStart;
};

struct PointLight {
    vec3 position;
    vec3 color;
    
    float linear;
    float quadratic;
    float radius;
};

layout (std430, binding = 0) restrict buffer clusterSSBO {
    Cluster b_Clusters[];
};

layout (std430, binding = 1) restrict buffer lightSSBO {
    PointLight b_Lights[];
};

layout(std430, binding = 2) restrict buffer lightIndicesSSBO {
    uint b_PointLightIndicies[];
};

struct BVHNode {
    vec4 minPoint;
    vec4 maxPoint;
    uint first_child_or_primitive;
    uint primitive_count;
};

layout(std430, binding = 3) restrict buffer bvhNodesSSBO {
    BVHNode b_BvhNodes[];
};

layout(std430, binding = 4) restrict buffer bvhIndicesSSBO {
    uint b_BvhIndices[];
};

layout(std430, binding = 5) buffer materialBuffer {
    uint b_MaterialData[];
};

struct MaterialMeta {
    uint offset;
    uint size;
};

layout (std430, binding = 6) buffer materialMetaSSBO {
    MaterialMeta b_MaterialMeta[];
};

uniform uint  u_CurrentMaterialStartId;
uniform float u_Time;
uniform float u_DeltaTime;
uniform uint  u_Frame;
uniform ivec2 u_Resolution;
uniform vec2  u_TexelSize;
uniform vec3  u_CameraPosition;
uniform vec3  u_CameraDirection;
uniform mat4  u_Model;
uniform mat4  u_View;
uniform mat4  u_Projection;
uniform mat4  u_InvModel;
uniform mat4  u_InvView;
uniform mat4  u_InvProjection;
uniform float u_ZNear;
uniform float u_ZFar;

{{ material_definition }}