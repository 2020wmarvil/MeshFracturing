#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 worldPos;
in vec2 texCoords;

uniform vec3 cameraPos;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    float shininess;
}; 
  
uniform Material material;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirectionalLight dirLight;

struct PointLight {    
    vec3 position;
    vec3 color;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight {    
    vec3 position;
    vec3 direction;
    vec3 color;

    float cutoff;
    float innerCutoff;
    float outerCutoff;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_SPOT_LIGHTS 1
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 color = vec3(0.0);
    
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(cameraPos - worldPos);
    
    color += CalcDirLight(dirLight, norm, viewDir);

    //for(int i = 0; i < NR_POINT_LIGHTS; i++) {
    //	color += CalcPointLight(pointLights[i], norm, worldPos, viewDir);
    //}

    //for(int i = 0; i < NR_SPOT_LIGHTS; i++) {
    //	color += CalcSpotLight(spotLights[i], norm, worldPos, viewDir);
    //}
    
    FragColor = vec4(color, 1.0);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse1, texCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse1, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular1, texCoords));
    return (ambient + diffuse + specular) * light.color;
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse1, texCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse1, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular1, texCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular) * light.color;
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = acos(dot(lightDir, normalize(-light.direction))) * 180.0 / 3.1415;

    float epsilon   = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.innerCutoff) / epsilon, 0.0, 1.0); 

    vec3 ambient  = vec3(0.0);
    vec3 diffuse  = vec3(0.0);
    vec3 specular = vec3(0.0);
        
    if(theta < light.cutoff) {       
        // diffuse shading
        float diff = max(dot(normal, lightDir), 0.0);
        // specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse1, texCoords));
        specular = light.specular * spec * vec3(texture(material.specular1, texCoords));
    }

    ambient  = light.ambient  * vec3(texture(material.diffuse1, texCoords));

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    return (ambient + diffuse + specular) * attenuation * intensity * light.color;
}
