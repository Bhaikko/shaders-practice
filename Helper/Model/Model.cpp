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
        glDrawArrays(GL_TRIANGLES, 0, faces.size());   
    glBindVertexArray(0);
    
}

void Model::GenerateTangents()
{
    std::vector<glm::vec3> tan1Accum(vertices.size()); 
    std::vector<glm::vec3> tan2Accum(vertices.size()); 
    tangents.resize(vertices.size());

    for (unsigned int i = 0; i < faces.size(); i+= 3) {
        const glm::vec3& p1 = vertices[faces[i].vertexIndex];
        const glm::vec3& p2 = vertices[faces[i + 1].vertexIndex];
        const glm::vec3& p3 = vertices[faces[i + 2].vertexIndex];

        const glm::vec2& tc1 = uvs[faces[i].textureCoordIndex];
        const glm::vec2& tc2 = uvs[faces[i + 1].textureCoordIndex];
        const glm::vec2& tc3 = uvs[faces[i + 2].textureCoordIndex];

        glm::vec3 q1 = p2 - p1;
        glm::vec3 q2 = p3 - p1;

        float s1 = tc2.x - tc1.x, s2 = tc3.x - tc1.x;
        float t1 = tc2.y - tc1.y, t2 = tc3.y - tc1.y;
        float r = 1.0f / (s1 * t2 - s2 * t1);

        glm::vec3 tan1( 
            (t2*q1.x - t1*q2.x) * r,
            (t2*q1.y - t1*q2.y) * r,
            (t2*q1.z - t1*q2.z) * r
        );

        glm::vec3 tan2( 
            (s1*q2.x - s2*q1.x) * r,
            (s1*q2.y - s2*q1.y) * r,
            (s1*q2.z - s2*q1.z) * r
        );

        tan1Accum[faces[i].vertexIndex] += tan1;
        tan1Accum[faces[i + 1].vertexIndex] += tan1;
        tan1Accum[faces[i + 2].vertexIndex] += tan1;

        tan2Accum[faces[i].vertexIndex] += tan2;
        tan2Accum[faces[i + 1].vertexIndex] += tan2;
        tan2Accum[faces[i + 2].vertexIndex] += tan2;
    }

    for (GLuint i = 0; i < vertices.size(); i++) {
        const glm::vec3& n = normals[i];
        glm::vec3& t1 = tan1Accum[i];
        glm::vec3& t2 = tan2Accum[i];

        // Gram-Schmidt Orthogonalize
        tangents[i] = glm::vec4(glm::normalize(t1 - (glm::dot(n, t1) * n)), 0.0f);

        // Store handedness in w
        tangents[i].w = (glm::dot(glm::cross(n, t1), t2) < 0.0f ? -1.0f : 1.0f);
    }


    // Allocating Tangents
    GLuint VBO;
    GLfloat* data = nullptr;

    glBindVertexArray(VAO); 
        // Binding Normal Data
        data = new GLfloat[faces.size() * 4];
        unsigned int index = 0;

        for (unsigned long long i = 0; i < faces.size(); i++) {
            data[index++] = tangents[faces[i].normalIndex].x;
            data[index++] = tangents[faces[i].normalIndex].y;
            data[index++] = tangents[faces[i].normalIndex].z;
            data[index++] = tangents[faces[i].normalIndex].w;
        }

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, faces.size() * sizeof(data[0]) * 4, data, GL_STATIC_DRAW);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(data[0]) * 4, 0);
            glEnableVertexAttribArray(3);
        
        delete data;
        index = 0;

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);    
}

Model::~Model()
{
    
}