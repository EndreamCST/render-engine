#version 330 core
out vec4 fragColor;

in FROM_VS_TO_FS {
    vec2 texCoords;
    vec3 worldPos;
    vec3 normal;
    vec4 lightSpacePos;
} frag;

uniform sampler2D diffuseTexture;
//uniform sampler2D specularTexture;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;


void main(){
    //result
    vec3 result = vec3(0);
    //light
    //vec3 lightColor = vec3(1.0);
    float lightPower = 10.0f;

    //material
    vec3 diffuseMaterial = texture(diffuseTexture, frag.texCoords).rgb;
    //vec3 specularMaterial = texture(specularTexture, uv).rgb;
    //compute
    vec3 n = normalize(frag.normal);
    vec3 l = normalize(lightPos-frag.worldPos);
    vec3 v = normalize(viewPos-frag.worldPos);
    vec3 h = normalize(l+v);
    float cosTheta = clamp(dot(n,l),0,1);
    float cosAlpha = clamp(dot(n,h),0,1);
    float d = length(lightPos-frag.worldPos);
    float attenuation = 1.0/(d);
    //color
    vec3 ambientColor = diffuseMaterial*vec3(0.1)*lightColor;
    vec3 diffuseColor = diffuseMaterial*lightColor*cosTheta;
    //vec3 specularColor = specularMaterial*lightColor*pow(cosAlpha,16)*vec3(0.5);


    result += ambientColor+diffuseColor*attenuation*lightPower;
    fragColor = vec4(result, 1.0);
    //fragColor = vec4(1.0);
}