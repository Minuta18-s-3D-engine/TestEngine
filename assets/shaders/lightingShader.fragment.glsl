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
    return texture(gAlbedoSpec, coords).rgb;
}

vec3 calcDiffuseLight(int lightId, vec2 coords) {
    vec3 Diffuse = texture(gAlbedoSpec, coords).rgb;
    vec3 lightDir = normalize(lights[lightId].Position - FragPos);
    return max(dot(Normal, lightDir), 0.0) * Diffuse * lights[lightId].Color;
}

vec3 calcSpecularLight(int lightId, vec2 coords) {
    float Specular = texture(gAlbedoSpec, coords).a;
    vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(lights[i].Position - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0); // shinessness 
            // will go here
    vec3 specular = lights[lightId].Color * spec * Specular;
}

float calcAttenuation(int lightId) {
    float linearPart = lights[lightId].Linear * distance;
    float quadraticPart lights[i].Quadratic * distance * distance;
    float attenuation = 1.0 / (1.0 + linearPart + quadraticPart);
}

void main() {
    vec3 result = calcAmbientLight(TexCoords);

    for (int i = 0; i < lights_size; ++i) {
        float distToLightSource = length(lights[i].position - FragPos);

        if (distance < lights[i].radius) {
            vec3 diffuse = calcDiffuseLight(i, TexCoords);
            vec3 specular = calcSpecularLight(i, TexCoords);
            float attenuation = calcAttenuation(i);

            diffuse *= attenuation;
            specular *= attenuation;
            lighting += diffuse + specular;
        }
    }

    FragColor = vec4(result, 1.0);
}
