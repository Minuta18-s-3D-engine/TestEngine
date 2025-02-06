#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    float shininess;
};

struct Light {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// struct Texture {
//     sampler2D texture;
//     float scaleX, scaleY;
//     float shiftX, shiftY;
//     bool activeTexture;
// }

in vec3 Normal;  
in vec3 FragPos;  
  
uniform Material material;
uniform Light light;
uniform vec3 viewPos; 

// uniform Texture objectTexture;

vec3 calcAmbientLight() {
    return light.ambient * material.ambient;
}

vec3 calcDiffuseLight() {
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.pos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    return light.diffuse * (diff * material.diffuse);
}

vec3 calcSpecularLight() {
    vec3 lightDir = normalize(light.pos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 normal = normalize(Normal);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    return light.specular * (spec * material.specular);
}

void main() {
    vec3 ambient = calcAmbientLight();
    vec3 diffuse = calcDiffuseLight();
    vec3 specular = calcSpecularLight();
        
    vec3 result = (ambient + diffuse + specular) * material.color;
    FragColor = vec4(result, 1.0);
} 