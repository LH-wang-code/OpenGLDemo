vec3 calculateAmbient(vec3 lightAmbient,vec3 materialAmbient)
{
	return lightAmbient * materialAmbient;
}