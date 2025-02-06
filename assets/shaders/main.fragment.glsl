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

struct TextureMaterial {
    sampler2D texture;
    float scaleX, scaleY;
    float shiftX, shiftY;
    bool isActive;
};

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;
  
uniform Material material;
uniform Light light;
uniform vec3 viewPos; 

uniform TextureMaterial objectTexture; // Which is also diffuse map
uniform TextureMaterial specularMapTexture;

vec3 calcAmbientLight() {
    return light.ambient * material.ambient;
}

vec3 calcDiffuseLight() {
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.pos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    return light.diffuse * (diff * material.diffuse);
}

vec3 getTexturePixel(TextureMaterial tex, vec2 coords) {
    if (!tex.isActive) return vec3(0.0, 0.0, 0.0);
    return vec3(texture(tex.texture, coords));
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
    vec3 result;

    if (!objectTexture.isActive) {
        vec3 ambient = calcAmbientLight();
        vec3 diffuse = calcDiffuseLight();
        vec3 specular = calcSpecularLight();

        result = (ambient + diffuse + specular) * material.color;
    } else if (objectTexture.isActive && !specularMapTexture.isActive) {
        vec3 ambient = calcAmbientLight() * getTexturePixel(objectTexture, TexCoords);
        vec3 diffuse = calcDiffuseLight() * getTexturePixel(objectTexture, TexCoords);;
        vec3 specular = calcSpecularLight() * getTexturePixel(objectTexture, TexCoords);;

        result = (ambient * + diffuse + specular);
    } else {
        vec3 ambient = calcAmbientLight() * getTexturePixel(objectTexture, TexCoords);
        vec3 diffuse = calcDiffuseLight() * getTexturePixel(objectTexture, TexCoords);;
        vec3 specular = calcSpecularLight() * getTexturePixel(specularMapTexture, TexCoords);

        result = (ambient + diffuse + specular);
    }
    FragColor = vec4(result, 1.0);
} 