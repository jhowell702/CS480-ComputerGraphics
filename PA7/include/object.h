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
    void Update(unsigned int dt, float simSpeed);
    void Render();
    void RenderTextures();

    void loadDefaultCube();

    void setDir(bool newState);
    void setSpin(bool newState);
    bool getDir();
    bool getSpin();
    std::string getName();

    void setName(std::string set){name = set;};
    void setSpinSpeed(float set){spinSpeed = set * 10000;};
    void setRotSpeed(float set){rotSpeed = set * 1500;};
    void setRadius(float set){radius = set * 5;};
    void setScale(float set){scale = set;};
    void setParent(std::string set){parent = set;};

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
     
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint TB;

    std::vector<float> TextureCoords;

    std::vector<unsigned int> textureIDs;
    

    float currSpinAngle;
    float currRotAngle;

    float spinSpeed;
    float rotSpeed;
    float radius;
    float scale;
    std::string parent;
  

    unsigned int matInd;

};

#endif /* OBJECT_H */
