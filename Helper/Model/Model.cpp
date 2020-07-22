#include "Model.h"

Model::Model()
{
    VAO = 0;
    VBO = 0;
    IBO = 0;
    verticesCount = 0;
}

void Model::LoadModel(const char* filePath)
{
    std::ifstream in(filePath, std::ios::in);
    if (!in) {
        std::cerr << "Cannot open " << filePath << std::endl;
        exit(1);
    }

    std::string line;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::vector<GLfloat> meshVertices;


    while (std::getline(in, line)) {

        if (line.substr(0,2) == "v ") {
            
            std::istringstream v(line.substr(2));

            glm::vec3 vert;
            double x, y, z;
            v >> x; v >> y; v >> z;
            vert = glm::vec3(x,y,z);
            vertices.push_back(vert);

        } else if(line.substr(0,2)=="vt"){
            
            std::istringstream v(line.substr(3));

            glm::vec2 tex;
            double U, V;
            v >> U; v >> V;
            tex = glm::vec2(U,V);
            uvs.push_back(tex);

        } 
        else if (line.substr(0, 2) == "vn") {
            std::istringstream v(line.substr(3));

            glm::vec3 norm;
            double x, y, z;
            v >> x; v >> y; v >> z;
            norm = glm::vec3(x, y, z);
            normals.push_back(norm);

        } 
        else if(line.substr(0,2)=="f ") {
            int v1, v2, v3; 
            int uv1, uv2, uv3; 
            int norm1, norm2, norm3;
            const char* chh=line.c_str();
            sscanf (chh, "f %i/%i/%i %i/%i/%i %i/%i/%i", &v1, &uv1, &norm1, &v2, &uv2, &norm2, &v3, &uv3, &norm3); 
            v1--; v2--; v3--;
            uv1--; uv2--; uv3--;
            norm1--; norm2--; norm3--;

            meshVertices.push_back(vertices[v1].x);
            meshVertices.push_back(vertices[v1].y);
            meshVertices.push_back(vertices[v1].z);
            meshVertices.push_back(uvs[uv1].x);
            meshVertices.push_back(uvs[uv1].y);
            meshVertices.push_back(normals[norm1].x);
            meshVertices.push_back(normals[norm1].y);
            meshVertices.push_back(normals[norm1].z);

            meshVertices.push_back(vertices[v2].x);
            meshVertices.push_back(vertices[v2].y);
            meshVertices.push_back(vertices[v2].z);
            meshVertices.push_back(uvs[uv2].x);
            meshVertices.push_back(uvs[uv2].y);
            meshVertices.push_back(normals[norm2].x);
            meshVertices.push_back(normals[norm2].y);
            meshVertices.push_back(normals[norm2].z);

            meshVertices.push_back(vertices[v3].x);
            meshVertices.push_back(vertices[v3].y);
            meshVertices.push_back(vertices[v3].z);
            meshVertices.push_back(uvs[uv3].x);
            meshVertices.push_back(uvs[uv3].y);
            meshVertices.push_back(normals[norm3].x);
            meshVertices.push_back(normals[norm3].y);
            meshVertices.push_back(normals[norm3].z);
        }
    }

    verticesCount = meshVertices.size();
    meshData = new GLfloat[verticesCount];
    for (long long i = 0; i < meshVertices.size(); i++) {
        meshData[i] = meshVertices[i];
    }

    LoadModelDataIntoBuffers();
}

void Model::LoadModelDataIntoBuffers()
{
    glGenVertexArrays(1, &VAO); 
    glBindVertexArray(VAO); 

        glGenBuffers(1, &VBO);  
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(meshData[0]), meshData, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(meshData[0]) * 8, 0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(meshData[0]) * 8, (void*)(sizeof(meshData[0]) * 3));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(meshData[0]) * 8, (void*)(sizeof(meshData[0]) * 5));
            glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);   

    glBindVertexArray(0);
}

void Model::RenderModel()
{
    glBindVertexArray(VAO);    
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
            glDrawArrays(GL_TRIANGLES, 0, verticesCount);   
        // glDisable(GL_CULL_FACE);
    glBindVertexArray(0);
    
}

Model::~Model()
{
    
}