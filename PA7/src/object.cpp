#include "object.h"


#include <fstream>
#include <sstream>

Object::Object()
{  
  loadDefaultCube();

  name = "Default Cube, no model file name given in command line";
  Init();

}

void Object::Init(){

    currRotAngle = 0.0f;
    currSpinAngle = 0.0f;

//    rotSpeed = 1500;
//    spinSpeed = 1000;

    location = glm::mat4(1.0f);

    parent = "none";

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

   //repeat and make texture coord buffer for use in shaders
   glGenBuffers(1, &TB);
   glBindBuffer(GL_ARRAY_BUFFER, TB);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * TextureCoords.size(), &TextureCoords[0], GL_STATIC_DRAW);

}


Object::Object(aiMesh *mesh)
{

////////////////////////////////////////////////////////////////////////

	//temp vec3's to convert from assimp vec3's
	glm::vec3 tempVec;
	glm::vec3 tempColor;

	//for each face
	for(unsigned int i=0;i<mesh->mNumFaces;i++){
		const aiFace& face = mesh->mFaces[i];
		
		//push all face indices
		for(int c = 0; c < face.mNumIndices; c++){
			Indices.push_back(face.mIndices[c]);
		}			

		//for each point on a face
		for(int j=0;j<3;j++){

			//get texture coords as offset by current face
			aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
			//push back onto vector for uv coords
			TextureCoords.push_back(uv.x);
			TextureCoords.push_back(uv.y);
	
			//get vertices coords as offset by current face
			aiVector3D pos = mesh->mVertices[face.mIndices[j]];
			//convert to glvec3
			tempColor = {1.0f, 1.0f, 1.0f};
			tempVec = {pos.x, pos.y, pos.z};

			Vertex tempVertex = {tempVec, tempColor};
			//push_back onto Vertices each vertex
			Vertices.push_back(tempVertex);	
		}
	}

	//get int index of which material/texture to use
	matInd = mesh->mMaterialIndex;

	//default init
	Init();
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::loadDefaultCube(){
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
    {{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
    {{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}}
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

}

void Object::setDir(bool newState){
}

bool Object::getDir(){
}

bool Object::getSpin(){
}

void Object::setSpin(bool newState){
}

void Object::Update(unsigned int dt, float simSpeed)
{

	//update spin angles depending on boolean
  	currSpinAngle += dt * M_PI/spinSpeed;
  	currRotAngle += dt * M_PI/rotSpeed;


	//translate model location based on x and z coords as calculated to be on circle

	location = glm::translate(glm::vec3(radius * (glm::cos(currRotAngle)),
						 0,
						radius * (glm::sin(currRotAngle))));

	//rotate model matrix at identity matrix for spin
    	model = glm::rotate(location, (currSpinAngle), glm::vec3(0.0, 1.0, 0.0));

	model = glm::scale(model, glm::vec3(scale, scale, scale));
}

glm::mat4 Object::GetModel()
{
  return model;
}

glm::mat4 Object::GetLocation(){
	return location;
}

void Object::SetScale(glm::vec3 newScale){
}

void Object::SetParent(Object* target){
}

void Object::IncrementRotationSpeed(int change){
	rotSpeed += change;
}

int Object::GetRotationSpeed(){
	return rotSpeed;
}

void Object::IncrementSpinSpeed(int change){
	spinSpeed += change;
}

int Object::GetSpinSpeed(){
	return spinSpeed;
}

std::string Object::getName(){
	return name;
}


void Object::RenderTextures()
{

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  //set active texture to by tex unit 0
  glActiveTexture(GL_TEXTURE0);

  //bind active texture to be texture saved by opengl at material index
  glBindTexture(GL_TEXTURE_2D, matInd);
  
  //bind texture coord buffer for shader
  glBindBuffer(GL_ARRAY_BUFFER, TB);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float), 0);
 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);

}

