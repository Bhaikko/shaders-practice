#pragma once

#include "./../../../Helper/common_headers.h"

class Parallax
{
    private:
        Shader shader;
        Model plane;
        Texture colorMap, normalMap, heightMap;

    public:
        Parallax();
        void Init();
        void Update(float deltaTime);
        void Render(glm::mat4 view, glm::mat4 projection);
};

/* 
    Height Map Theory

    Each pixel store values, such as surface elevation data, for display in 3D computer graphics. 
    A heightmap can be used in bump mapping to calculate where this 3D data would create shadow in a material, 
    in displacement mapping to displace the actual geometric position of points over the textured surface, 
    or for terrain where the heightmap is converted into a 3D mesh.

    A Grayscale image, with black representing minimum height and white representing maximum height having 8 bit data.
 */