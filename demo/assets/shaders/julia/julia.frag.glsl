layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 v_WorldPos;
in vec2 v_UV;
in vec3 v_Normal;
in vec3 v_Tangent;
in vec3 v_Bitangent;

vec3 get_palette(float t) {
    if (t <= 0.5) return mix(vec3(1.0, 1.0, 1.0), vec3(0.2, 0.2, 0.2), t * 2);
    return mix(vec3(0.2, 0.2, 0.2), vec3(0.0, 0.0, 0.0), (t - 0.5) * 2);
}

void fragment() {
    float zoom = 0.5; // currentMaterial.zoom

    gPosition = v_WorldPos;
    vec3 normal = normalize(v_Normal);
    gNormal = normalize(normal);

    vec2 uv = (gl_FragCoord.xy - u_Resolution * 0.5) / 
        min(u_Resolution.x, u_Resolution.y);
    vec2 z = uv * (3.0 / zoom); 


    // vec2 c = vec2(secTime, secTime + 1.0);
    vec2 c = vec2(-0.7, 0.27015) + vec2(cos(u_Time * 0.25), sin(u_Time * 1.3 * 0.25)) * 0.2;

    uint n = 0;
    const uint max_iterations = 256;
    
    for (uint i = 0; i < max_iterations; ++i) {
        float x_squared = z.x * z.x;
        float y_squared = z.y * z.y;
        
        if (x_squared + y_squared > 4.0) {
            n = i;
            break;
        }
        
        z = vec2(x_squared - y_squared, 2.0 * z.x * z.y) + c;
    }

    vec3 color;
    if (n >= max_iterations) {
        color = vec3(0.0);
    } else {
        float log_zn = log(z.x * z.x + z.y * z.y) / 2.0;
        float nu = log(log_zn / log(2.0)) / log(2.0);
        float smooth_t = (n + 1.0 - nu) / max_iterations;
        
        color = get_palette(smooth_t); 
    }

    gAlbedoSpec = vec4(color, 1.0);
}
