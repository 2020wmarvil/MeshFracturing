#version 330 core

out vec4 FragColor;

in vec3 worldPos;
in vec3 normal;

uniform vec3 wireColor;
uniform float bFlat;

const vec3 lightPos 	= vec3(200,60,100);
const vec3 ambientColor = vec3(0.2, 0.0, 0.0);
const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
const vec3 specColor 	= vec3(1.0, 1.0, 1.0);

void main() {
    vec3 wPos = vec3(model * vec4(pos, 1.0));
	vec3 norm = mix(normalize(normal), normalize(cross(dFdx(worldPos), dFdy(worldPos))), bFlat);
	vec3 lightDir = normalize(lightPos - worldPos);
	
	float lambertian = max(dot(lightDir,norm), 0.0);
	float specular = 0.0;
	
	if(lambertian > 0.0) {
		vec3 viewDir = normalize(-worldPos);
		vec3 halfDir = normalize(lightDir + viewDir);
		float specAngle = max(dot(halfDir, norm), 0.0);
		specular = pow(specAngle, 16.0);
	}
	
	gl_FragColor = vec4(ambientColor + lambertian * diffuseColor + specular * specColor, 1.0);
}
