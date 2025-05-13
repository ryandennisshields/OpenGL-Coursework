#version 400 

out vec4 FragColor; 
in vec3 v_norm; 
in vec4 v_pos; // Assuming this is in camera space 

uniform vec3 fogColor; 
uniform vec3 cameraPos; // Camera position in the same space as v_pos 

uniform float maxDist; // Fog max distance 
uniform float minDist; // Fog min distance 

void main() 
{ // Calculate the full 3D distance from the camera to the vertex 
float dist = length(v_pos.xyz - cameraPos); // Compute the fog factor based on the distance 
float fogFactor = (maxDist - dist) / (maxDist - minDist); 
fogFactor = clamp(fogFactor, 0.0, 1.0); // Base color of the light (assuming white light for simplicity) 
vec3 lightColor = vec3(0.8, 0.8, 0.8); // Calculate the final color with the fog effect 
vec3 color = mix(fogColor, lightColor, 1 - fogFactor); // Output the final color 

FragColor = vec4(color, 1.0); 

}
