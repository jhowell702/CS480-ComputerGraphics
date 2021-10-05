#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    Object(aiMesh *mesh);
    ~Object();

    void Init();
    void Update(unsigned int dt, int radius);
    void Render();
    void RenderTextures();

    void loadDefaultCube();

    void setDir(bool newState);
    void setSpin(bool newState);
    bool getDir();
    bool getSpin();

    std::string getName();

    glm::mat4 GetModel();
    glm::mat4 GetLocation();

    void SetScale(glm::vec3);

    void SetParent(Object* target);

    void IncrementRotationSpeed(int change);
    int GetRotationSpeed();

    void IncrementSpinSpeed(int change);
    int GetSpinSpeed();
    
    bool hasColor;

  private:
    std::string name;

    glm::mat4 model;
    std::vector<Vertex> Vertices;

    glm::mat4 location;
    glm::vec3 scale;
     
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint TB;

    std::vector<float> TextureCoords;

    std::vector<unsigned int> textureIDs;
    
    Object* parent;

    bool dirFlag;
    bool spinFlag;

    float spinAngle;
    float rotationAngle;

    int spinSpeed;
    int rotationSpeed;

    unsigned int texture;
 
    int numVerts;

    unsigned int matInd;

};

#endif /* OBJECT_H */
