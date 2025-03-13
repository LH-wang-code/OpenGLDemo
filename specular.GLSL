vec3 calculateSpecular(vec3 lightSpecular,vec3 materialSpecular,float shininess,vec3 normal,vec3 lightDir,vec3 viewDir)
{
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.0f),shininess);
	return lightSpecular * (spec * materialSpecular);
}