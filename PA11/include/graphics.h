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

    Object * LoadObjects(std::string fileName, btScalar mass, btVector3 startPos, std::string meshType, glm::vec4 cubeSize);
    void LoadFromConfig(std::string* fileNames);
    unsigned int * loadTextures(std::string texFileName);

    std::string objName;

    Object * getCube(){		std::string cube = "Cube";
				return m_objects[cube];};


    Camera * getCamera(){return m_camera;};
    Object * getObject(std::string name){return m_objects[name];};

	void incLight(){if(shaderSwitch == 0){lightPos.x = lightPos.x + 50;}
			   		 else{lightPos.x = lightPos.x - 50;}};
	void decLight(){if(shaderSwitch == 0){lightPos.x = lightPos.x - 50;}
			   		 else{lightPos.x = lightPos.x + 50;}};
	void incLightZ(){if(shaderSwitch == 0){lightPos.z = lightPos.z + 50;}
			   		 else{lightPos.z = lightPos.z - 50;}};
	void decLightZ(){if(shaderSwitch == 0){lightPos.z = lightPos.z - 50;}
			   		 else{lightPos.z = lightPos.z + 50;}};
	void incLightY(){lightPos.y = lightPos.y + 50;};
	void decLightY(){lightPos.y = lightPos.y - 50;};
	void reverseLights(){lightPos.x = -lightPos.x;
				  lightPos.z = -lightPos.z;};

	void increaseAmbient(){ambientVec.x += .1;ambientVec.y += .1;ambientVec.z += .1;}
	void decreaseAmbient(){ambientVec.x -= .1;ambientVec.y -= .1;ambientVec.z -= .1;}

	void increaseSpecular(){specularVec.x += .1;specularVec.y += .1;specularVec.z += .1;};
	void decreaseSpecular(){specularVec.x -= .1;specularVec.y -= .1;specularVec.z -= .1;};
	
	void light1on(){diffuseVec = glm::vec4(0,.3,.8,1);};
	void light2on(){diffuseVec2 = glm::vec4(0,0,1,1);};
	void light1off(){diffuseVec = glm::vec4(0,0,0,1);};
	void light2off(){diffuseVec2 = glm::vec4(0,0,0,1);};

	glm::vec4 getLightPos(){return lightPos;};

   int shaderSwitch;

   bool SetUniforms();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_ambientProduct;
    GLint m_diffuseProduct;
    GLint m_specularProduct;
    GLint m_lightPos;
    GLint m_shininess;

    glm::vec4 ambientVec;
    glm::vec4 diffuseVec;
    glm::vec4 specularVec;
    glm::vec4 lightPos;
    float shininess;

    GLint m_diffuseProduct2;
    GLint m_lightPos2;
    glm::vec4 lightPos2;
    glm::vec4 diffuseVec2;

    float spinSimSpeed;
    float rPSimSpeed;
    float rMSimSpeed;

btDiscreteDynamicsWorld *dynamicsWorld;


    map<std::string, Object * > m_objects;
    std::vector<Object *> l_objects;

};

#endif /* GRAPHICS_H */
