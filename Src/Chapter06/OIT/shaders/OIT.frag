#version 450

// Enabling early z-testing
layout (early_fragment_tests) in;

#define MAX_FRAGMENTS 75

in vec3 Position;
in vec3 Normal;

uniform vec4 LightPosition;
uniform vec3 LightIntensity;

uniform vec4 Kd;
uniform vec4 Ka;

struct NodeType {
    vec4 color;
    float depth;
    uint next;
};

// Image Load Store
// Shaders can read information from these images and write information to them,
// in ways that they cannot with textures.
layout (binding = 0, r32ui) uniform uimage2D headPointers;
layout (binding = 0, offset = 0) uniform atomic_uint nextNodeCounter;
layout (binding = 0, std430) buffer linkedLists {
    NodeType nodes[];
};

uniform uint MaxNodes;

layout (location = 0) out vec4 FragColor;

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

vec3 Diffuse()
{
    vec3 s = normalize(LightPosition.xyz - Position);
    vec3 n = normalize(Normal);

    return LightIntensity * (Ka.rgb + Kd.rgb * max(dot(s, n), 0.0));

}

subroutine(RenderPassType)
void Pass1()
{
    // Get index of next empty slot in buffer
    uint nodeIdx = atomicCounterIncrement(nextNodeCounter);

    // If buffer is not full
    if (nodeIdx < MaxNodes) {
        // atomically store supplied data into memory and return the original value from memory
        // This will return already existing head from linked list and insert new index
        uint prevHead = imageAtomicExchange(headPointers, ivec2(gl_FragCoord.xy), nodeIdx);

        // Setting up color, depth and next pointer
        nodes[nodeIdx].color = vec4(Diffuse(), Kd.a);
        nodes[nodeIdx].depth = gl_FragCoord.z;
        nodes[nodeIdx].next = prevHead;
    }
}

subroutine(RenderPassType)
void Pass2()
{
    NodeType frags[MAX_FRAGMENTS];
    int count = 0;

    // Get index of head of linked list
    uint n = imageLoad(headPointers, ivec2(gl_FragCoord.xy)).r;

    // Copy the linked list for this fragment into an array
    while (n != 0xffffffff && count < MAX_FRAGMENTS) {
        frags[count] = nodes[n];
        n = frags[count].next;
        count++;
    }

    // Sorting the array by depth using insertion sort
    for (uint i = 1; i < count; i++) {
        NodeType toInsert = frags[i];
        uint j = i;
        while (j > 0 && toInsert.depth > frags[j - 1].depth) {
            frags[j] = frags[j - 1];
            j--;
        }

        frags[j] = toInsert;
    }

    // Traverse the array and combine the colors using alpha
    vec4 color = vec4(0.5, 0.5, 0.5, 1.0);
    for (int i = 0; i < count; i++) {
        color = mix(color, frags[i].color, frags[i].color.a);
    }
}

void main()
{
    RenderPass();
}