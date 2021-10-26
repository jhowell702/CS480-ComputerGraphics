#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, std::string* fileNames,     btDiscreteDynamicsWorld *dynamicsWorld);
    void Update(unsigned int dt);
    void Render();

    Object * LoadObjects(std::string fileName, btScalar mass, btVector3 startPos, std::string meshType);
    void LoadFromConfig(std::string* fileNames);
    unsigned int * loadTextures(std::string texFileName);

    std::string objName;

    Object * getCube(){		std::string cube = "Cube";
				return m_objects[cube];};

    Camera * getCamera(){return m_camera;};
    Object * getObject(std::string name){return m_objects[name];};

	void incSpinSim(float in){spinSimSpeed += in;};
	void incRotPlanSim(float in){rPSimSpeed += in;};
	void incRotMoonSim(float in){rMSimSpeed += in;};

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    float spinSimSpeed;
    float rPSimSpeed;
    float rMSimSpeed;

    map<std::string, Object * > m_objects;
    std::vector<Object *> l_objects;

};

#endif /* GRAPHICS_H */
