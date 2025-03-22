#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform vec3 viewPos;
uniform float shininess; // 独立变量替代Material结构体简化问题

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

float ShadowCalculation(vec4 fragPosLightSpace) {
    // 透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 转换到 [0,1] 范围
    projCoords = projCoords * 0.5 + 0.5;
    
    // 边界检查
    if (projCoords.z > 1.0) return 0.0;
    
    // 获取深度值
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    
    // 计算动态偏移
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(-dirLight.direction);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    
    // PCF采样
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    
    return shadow;
}

void main() {
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    
    // 计算阴影
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    
    // 漫反射颜色
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    
    // 环境光
    vec3 ambient = dirLight.ambient * color;
    
    // 漫反射
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = dirLight.diffuse * diff * color;
    
    // 镜面高光（简化版）
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = dirLight.specular * spec * vec3(0.5);
    
    // 最终颜色
    vec3 result = ambient + (1.0 - shadow) * (diffuse + specular);
    FragColor = vec4(result, 1.0);
}