vec3 calculateDiffuse(vec3 lightDiffuse,vec3 materialDiffuse,vec3 normal,vec3 lightDir)
{
	float diff=max(dot(normal,lightDir),0.0f);
	return lightDiffuse * (diff * materialDiffuse);
}