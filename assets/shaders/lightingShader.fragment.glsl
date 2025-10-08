#version 430
out vec4 FragColor;

struct Cluster {
    vec4 minPoint;
    vec4 maxPoint;
    uint count;
    uint lightIndices[100];
};


