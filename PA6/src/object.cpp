#include "object.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <fstream>
#include <sstream>

Object::Object()
{  
  loadDefaultCube();

  name = "Default Cube, no model file name given in command line";
  Init();

}

void Object::Init(){

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

   glGenBuffers(1, &TB);
   glBindBuffer(GL_ARRAY_BUFFER, TB);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * TextureCoords.size(), &TextureCoords[0], GL_STATIC_DRAW);

}

//custom constructor that takes a string file name to load a target object
Object::Object(std::string fileName)
{

	name = fileName;

////////////////////////////////////////////////////////////////////////

	glm::vec3 tempVec;
	glm::vec3 tempColor;

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile("../models/" + fileName,aiProcess_Triangulate);
 
	aiMesh *mesh = scene->mMeshes[0];

	if(mesh->HasVertexColors(0)){
		hasColor = true;
	}else{
		hasColor = false;
	}


	for(unsigned int i=0;i<mesh->mNumFaces;i++){
		const aiFace& face = mesh->mFaces[i];
		
		for(int c = 0; c < face.mNumIndices; c++){
			Indices.push_back(face.mIndices[c]);
		}			

		for(int j=0;j<3;j++){

			aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
			TextureCoords.push_back(uv.x);
			TextureCoords.push_back(uv.y);
	
			aiVector3D pos = mesh->mVertices[face.mIndices[j]];
			tempColor = {1.0f, 1.0f, 1.0f};
			tempVec = {pos.x, pos.y, pos.z};
			Vertex tempVertex = {tempVec, tempColor};
			//push_back onto Vertices each vertex
			Vertices.push_back(tempVertex);	
		}
	}


	loadTextures();
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

	//update spin angles depending on boolean
  	spinAngle += dt * M_PI/spinSpeed;

	//translate model location based on x and z coords as calculated to be on circle

	location = glm::translate(glm::vec3(radius * (glm::cos(rotationAngle)),
						 0,
						radius * (glm::sin(rotationAngle))));

	//rotate model matrix at identity matrix for spin
    model = glm::rotate(glm::mat4(1.0f), (spinAngle), glm::vec3(0.0, 1.0, 0.0));

	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
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

std::string Object::getName(){
	return name;
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

void Object::loadTextures(){

	int width, height, nrChannels;
	unsigned char *data = stbi_load("../models/checker.jpg", &width, &height, &nrChannels, 0); 

	std::cout << nrChannels;

	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture); 

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

}

void Object::RenderTextures()
{


  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));



  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);


  glBindBuffer(GL_ARRAY_BUFFER, TB);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float), 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);

}
