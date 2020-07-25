#include "Model.h"

Model::Model()
{
    VAO = 0;
}

void Model::LoadModel(const char* filePath)
{
    std::ifstream in(filePath, std::ios::in);
    if (!in) {
        std::cerr << "Cannot open " << filePath << std::endl;
        exit(1);
    }

    std::string line;

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

            FaceIndex faceIndexes[3];
            faceIndexes[0].vertexIndex = v1;
            faceIndexes[0].textureCoordIndex = uv1;
            faceIndexes[0].normalIndex = norm1;

            faceIndexes[1].vertexIndex = v2;
            faceIndexes[1].textureCoordIndex = uv2;
            faceIndexes[1].normalIndex = norm2;
            
            faceIndexes[2].vertexIndex = v3;
            faceIndexes[2].textureCoordIndex = uv3;
            faceIndexes[2].normalIndex = norm3;

            for (unsigned int i = 0; i < 3; i++) {
                faces.push_back(faceIndexes[i]);
            }


        }
    }

    LoadModelDataIntoBuffers();
}

void Model::LoadModelDataIntoBuffers()
{
    GLuint VBO;
    GLfloat* data = nullptr;


    glGenVertexArrays(1, &VAO); 
    glBindVertexArray(VAO); 

        // Binding Vertex Data
        data = new GLfloat[faces.size() * 3];
        unsigned int index = 0;
        for (unsigned long long i = 0; i < faces.size(); i++) {
            data[index++] = vertices[faces[i].vertexIndex].x;
            data[index++] = vertices[faces[i].vertexIndex].y;
            data[index++] = vertices[faces[i].vertexIndex].z;
        }

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 3 * faces.size() * sizeof(data[0]), data, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(data[0]) * 3, 0);
            glEnableVertexAttribArray(0);

        delete data;
        index = 0;
        
        // Binding UVs data
        data = new GLfloat[faces.size() * 2];
        for (unsigned long long i = 0; i < faces.size(); i++) {
            data[index++] = uvs[faces[i].textureCoordIndex].x;
            data[index++] = uvs[faces[i].textureCoordIndex].y;
        }

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, faces.size() * sizeof(data[0]) * 2, data, GL_STATIC_DRAW);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(data[0]) * 2, 0);
            glEnableVertexAttribArray(1);
        
        delete data;
        index = 0;

        // Binding Normal Data
        data = new GLfloat[faces.size() * 3];
        for (unsigned long long i = 0; i < faces.size(); i++) {
            data[index++] = normals[faces[i].normalIndex].x;
            data[index++] = normals[faces[i].normalIndex].y;
            data[index++] = normals[faces[i].normalIndex].z;
        }

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, faces.size() * sizeof(data[0]) * 3, data, GL_STATIC_DRAW);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(data[0]) * 3, 0);
            glEnableVertexAttribArray(2);
        
        delete data;
        index = 0;

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::RenderModel()
{
    glBindVertexArray(VAO);    
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
            glDrawArrays(GL_TRIANGLES, 0, faces.size());   
        // glDisable(GL_CULL_FACE);
    glBindVertexArray(0);
    
}

void Model::GenerateTangents()
{
    // std::vector<glm::vec3> tan1Accum(verticesCount); 
    // std::vector<glm::vec3> tan2Accum(verticesCount); 
    // tangents.resize(verticesCount);

    // for (unsigned int i = 0; i < facesCount; i++) {

    // }
}

Model::~Model()
{
    
}