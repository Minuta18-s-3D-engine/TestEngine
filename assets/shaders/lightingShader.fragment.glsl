#version 330 core
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

in vec2 TexCoords;
uniform vec3 viewPos;
vec3 FragPos = texture(gPosition, TexCoords).rgb;
vec3 Normal = texture(gNormal, TexCoords).rgb;

struct Light {
    vec3 position;
    vec3 color;
    
    float linear;
    float quadratic;
    float radius;
};

const int MAX_NR_LIGHTS = 1024;
uniform Light lights[MAX_NR_LIGHTS];
uniform int lights_size;

const float AMBIENT_LIGHT = 0.1; // Actuall not sure if I should get this 
                                 // from code.

vec3 calcAmbientLight(vec2 coords) {
    return texture(gAlbedoSpec, coords).rgb * AMBIENT_LIGHT;
}

vec3 calcDiffuseLight(int lightId, vec2 coords) {
    vec3 Diffuse = texture(gAlbedoSpec, coords).rgb;
    vec3 lightDir = normalize(lights[lightId].position - FragPos);
    return max(dot(Normal, lightDir), 0.0) * Diffuse * lights[lightId].color;
}

vec3 calcSpecularLight(int lightId, vec2 coords) {
    float Specular = texture(gAlbedoSpec, coords).a;
    vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(lights[lightId].position - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0); // shinessness 
            // will go here
    vec3 specular = lights[lightId].color * spec * Specular;
    return specular;
}

float calcAttenuation(int lightId, float distToLightSource) {
    float linearPart = lights[lightId].linear * distToLightSource;
    float quadraticPart = lights[lightId].quadratic * distToLightSource 
        * distToLightSource;
    float attenuation = 1.0 / (1.0 + linearPart + quadraticPart);
    return attenuation;
}

void main() {
    vec3 result = calcAmbientLight(TexCoords);

    for (int i = 0; i < lights_size; ++i) {
        float distToLightSource = length(lights[i].position - FragPos);

        // if (distToLightSource < lights[i].radius) {
            vec3 diffuse = calcDiffuseLight(i, TexCoords);
            vec3 specular = calcSpecularLight(i, TexCoords);
            float attenuation = calcAttenuation(i, distToLightSource);

            diffuse *= attenuation;
            specular *= attenuation;
            result += diffuse + specular;
        // }
    }

    FragColor = vec4(result, 1.0);
}
