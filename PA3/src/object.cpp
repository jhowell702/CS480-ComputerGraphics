#include "object.h"

Object::Object()
{  
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */

  Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };

  Indices = {
    2, 3, 4,
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }

  rotationAngle = 0.0f;
  spinAngle = 0.0f;

  rotationSpeed = 1500;
  spinSpeed = 1000;

  scale = glm::vec3(1,1,1);

  dirFlag = true;
  spinFlag = true;

  location = glm::mat4(1.0f);

  parent = NULL;

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::setDir(bool newState){
	dirFlag = newState;
}

bool Object::getDir(){
	return dirFlag;
}

bool Object::getSpin(){
	return spinFlag;
}

void Object::setSpin(bool newState){
	spinFlag = newState;
}

void Object::Update(unsigned int dt, int radius)
{

	//update rotation and spin angles depending on boolean 

	if(dirFlag){
  		rotationAngle += dt * M_PI/rotationSpeed;
	}else{
		rotationAngle -= dt * M_PI/rotationSpeed;
	}	

	if(spinFlag){
  		spinAngle += dt * M_PI/spinSpeed;
	}else{
  		spinAngle -= dt * M_PI/spinSpeed;
	}

	//translate model location based on x and z coords as calculated to be on circle

	location = glm::translate(glm::vec3(radius * (glm::cos(rotationAngle)),
						 0,
						radius * (glm::sin(rotationAngle))));

	//if target has a parent, then matrix multiply rotation offset by parent's location matrix
	//do nothing if parent pointer is NULL	

        if(parent == NULL){
		//do nothing
	}else{
		location = location * parent->GetLocation();
	}

	//rotate model matrix at transformation location for spin
        model = glm::rotate(location, (spinAngle), glm::vec3(0.0, 1.0, 0.0));

	model = glm::scale(model, scale);

}

glm::mat4 Object::GetModel()
{
  return model;
}

glm::mat4 Object::GetLocation(){
	return location;
}

void Object::SetScale(glm::vec3 newScale){
	scale = newScale;
}

void Object::SetParent(Object* target){
	parent = target;
}

void Object::IncrementRotationSpeed(int change){
	rotationSpeed += change;
}

int Object::GetRotationSpeed(){
	return rotationSpeed;
}

void Object::IncrementSpinSpeed(int change){
	spinSpeed += change;
}

int Object::GetSpinSpeed(){
	return spinSpeed;
}


void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

