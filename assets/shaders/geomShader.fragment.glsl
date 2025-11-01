#version 430 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

struct TextureMaterial {
    float scaleX, scaleY;
    float shiftX, shiftY;
    bool isActive;
};

uniform TextureMaterial textureDiffuse1Mat;
uniform TextureMaterial textureSpecular1Mat;
uniform sampler2D textureDiffuse1;
uniform sampler2D textureSpecular1;

void main() {    
    gPosition = FragPos;
    gNormal = normalize(Normal);
    gAlbedoSpec.rgb = texture(textureDiffuse1, TexCoords).rgb;
    gAlbedoSpec.a = texture(textureSpecular1, TexCoords).r;
}