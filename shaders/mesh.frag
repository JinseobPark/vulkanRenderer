#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec3 fragWorldPos;
layout(location = 3) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

void main() {
    // Simple directional lighting
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    vec3 normal = normalize(fragNormal);
    
    float ambient = 0.15;
    float diffuse = max(dot(normal, lightDir), 0.0);
    float lighting = ambient + diffuse * 0.85;
    
    vec3 color = fragColor * lighting;
    
    outColor = vec4(color, 1.0);
}
