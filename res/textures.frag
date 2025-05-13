#version 400

in vec2 tC;
in vec3 fP;
in mat3 TBN;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform sampler2D diffuseT;
uniform sampler2D normalT;
uniform sampler2D displacementT;
uniform sampler2D metallicT;
uniform float displacementScale;
uniform float opacity;	

out vec4 FragColour;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
	const float numLayers = 10;
	float layerDepth = 1.0 / numLayers;
	float currentLayerDepth = 0.0;
	vec2 P = viewDir.xy * displacementScale;
	vec2 deltatC = P / numLayers;
	vec2 currenttC = texCoords;
	float currentDisplacementValue = texture(displacementT, currenttC).r;

	while (currentLayerDepth < currentDisplacementValue)
	{
    currenttC -= deltatC;
    currentDisplacementValue = texture(displacementT, currenttC).r;  
    currentLayerDepth += layerDepth;
	}

return currenttC;
} 

void main()
{	
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);  

	vec2 texCoords = ParallaxMapping(tC, viewDir);

	vec3 colour = texture(diffuseT, texCoords).rgb;

	vec3 normal = texture(normalT, texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	float metallic = texture(metallicT, tC).r;

	vec3 ambient = 0.6 * colour;

	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuseT = diff * colour;

    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = mix(vec3(0.04), colour, metallic) * spec;

	FragColour = vec4(ambient + diffuseT + specular, opacity);
}
