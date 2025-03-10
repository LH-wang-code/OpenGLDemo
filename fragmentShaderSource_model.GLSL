#version 330 core
out vec4 FragColor;


struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


struct Light{
    vec3 position;
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Light lights[2];
uniform Material material;

uniform sampler2D texture_diffuse1;

void main()
{    

    vec3 result=vec3(0.0f);
    for(int i=0;i<2;i++)
    {
        vec3 ambient=lights[i].ambient * texture(material.diffuse,TexCoords).rgb;
        
        
        vec3 norm=normalize(Normal);
        vec3 lightDir=normalize(lights[i].position-FragPos);
        float diff=max(dot(norm,lightDir),0.0f);
        vec3 diffuse=lights[i].diffuse*diff*texture(material.diffuse,TexCoords).rgb;

        vec3 viewDir=normalize(viewPos-FragPos);
        vec3 reflectDir=reflect(-lightDir,norm);
        float spec=pow(max(dot(viewDir,reflectDir),0.0f),material.shininess);
        vec3 specular=lights[i].specular*spec * texture(material.specular,TexCoords).rgb;

        float distance=length(lights[i].position-FragPos);
        float attenuation=1.0f/(lights[i].constant+lights[i].linear*distance+lights[i].quadratic*(distance*distance));
        ambient*=attenuation;
        diffuse*=attenuation;
        specular*=attenuation;
        result+=ambient+diffuse+specular;
    }
    FragColor = vec4(result,1.0f) * texture(texture_diffuse1, TexCoords);
}