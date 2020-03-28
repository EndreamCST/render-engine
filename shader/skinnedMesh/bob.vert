#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vuv;
layout (location = 2) in vec3 vNormal;

layout (location = 3) in ivec4 vBoneIDs;
layout (location = 4) in vec4 vWeights;

out FROM_VS_TO_FS {
    vec2 texCoords;
    vec3 worldPos;
    vec3 normal;
    vec4 lightSpacePos;
} frag;

//layout (std140) uniform GlobalTransform {
uniform mat4 projection;
uniform mat4 view;
//};
uniform mat4 model;

const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];

uniform mat4 lightSpaceTransform;

void main(){
    mat4 BoneTransform = mat4(0.0);
    for (int i = 0; i < 4; i++) {
        BoneTransform += gBones[vBoneIDs[i]] * vWeights[i];
    }
    vec4 local_position = BoneTransform * vec4(vPosition, 1);
    vec4 local_normal = BoneTransform * vec4(vNormal, 0);

    frag.texCoords = vuv;
    frag.normal = vec3(model * local_normal);
    frag.worldPos = vec3(model * local_position);
    frag.lightSpacePos = lightSpaceTransform * vec4(frag.worldPos, 1.0);

    gl_Position = projection * view * model * local_position;
}