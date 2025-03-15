#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct DirLight {
    vec3 direction; // 定向光的方向

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform DirLight dirLight; 
uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 将NDC坐标转换到[0,1]范围
    projCoords = projCoords * 0.5 + 0.5;
    // 获取最近深度值
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // 当前片段的深度
    float currentDepth = projCoords.z;
    // 检查当前片段是否在阴影中
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow)
{
    // 环境光
    vec3 ambient = light.ambient * texture(material.diffuse, fs_in.TexCoords).rgb;

    // 漫反射
    vec3 lightDir = normalize(-light.direction); 
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, fs_in.TexCoords).rgb;

    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir),0.0f), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, fs_in.TexCoords).rgb;

    // 结合阴影
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

void main()
{           
    // 属性
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    // 计算阴影
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

    // 计算定向光
    vec3 result = CalcDirLight(dirLight, normal, viewDir, shadow);

    FragColor = vec4(result, 1.0);
}