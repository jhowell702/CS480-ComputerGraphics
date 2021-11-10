#include "object.h"
#include "graphics.h"

#include <fstream>
#include <sstream>

Object::Object()
{  
  loadDefaultCube();

  name = "Default Cube, no model file name given in command line";
  //Init(NULL);

}

void Object::Init(aiMesh * mesh, btScalar in_mass, btVector3 startPos){

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
   glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(GLfloat), mesh->mTextureCoords[0], GL_STATIC_DRAW);


}


Object::Object(aiMesh *mesh, unsigned int in_matInd, btScalar in_mass, btVector3 startPos, std::string meshType, glm::vec4 cubeSize)
{

////////////////////////////////////////////////////////////////////////

	if(meshType.compare("complex") == 0){
		createComplex(mesh, in_matInd, in_mass, startPos, meshType);
	}else if(meshType.compare("sphere") == 0){
		createSphere(mesh, in_matInd, in_mass, startPos, meshType);
	}else if(meshType.compare("cube") == 0){
		createCube(mesh, in_matInd, in_mass, startPos, meshType, cubeSize);
	}else if(meshType.compare("plane") == 0){
		createPlane(mesh, in_matInd, in_mass, startPos, meshType);
	}else if(meshType.compare("cylinder") == 0){
		createCylinder(mesh, in_matInd, in_mass, startPos, meshType);
	}

	score = 0;
	lives = 3;

////////////////////////////////////////////////////////////////////////

	
}

void Object::createSphere(aiMesh *mesh, unsigned int in_matInd, btScalar in_mass, btVector3 startPos, std::string meshType){

//temp vec3's to convert from assimp vec3's
	glm::vec3 tempVec;
	glm::vec3 tempNorm;

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
			aiVector3D norm = mesh->mNormals[face.mIndices[j]];
			//convert to glvec3
			tempVec = {pos.x, pos.y, pos.z};
			tempNorm = {norm.x, norm.y, norm.z};

			Vertex tempVertex = {tempVec, tempNorm};
			//push_back onto Vertices each vertex
			Vertices.push_back(tempVertex);	
		}
	}

	//get int index of which material/texture to use
	matInd = in_matInd;

/////////////////////////////////////////

    shape = new btSphereShape(1);

    btQuaternion rotation;
    rotation.setEulerZYX(0,0,0);
    
    //2
    btVector3 position = startPos;

    shapeMotionState = new btDefaultMotionState( btTransform(rotation, position) );     

    btScalar mass(in_mass);

    btVector3 inertia;

    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);

    rigidBody = new btRigidBody(shapeRigidBodyCI);

	rigidBody->setCcdMotionThreshold(1e-7);
	rigidBody->setCcdSweptSphereRadius(0.50);
//////////////////////////////////////

	//default init
	Init(mesh, mass, startPos);

}

void Object::createCylinder(aiMesh *mesh, unsigned int in_matInd, btScalar in_mass, btVector3 startPos, std::string meshType){

//temp vec3's to convert from assimp vec3's
	glm::vec3 tempVec;
	glm::vec3 tempNorm;

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
			aiVector3D norm = mesh->mNormals[face.mIndices[j]];
			//convert to glvec3
			tempVec = {pos.x, pos.y, pos.z};
			tempNorm = {norm.x, norm.y, norm.z};

			Vertex tempVertex = {tempVec, tempNorm};
			//push_back onto Vertices each vertex
			Vertices.push_back(tempVertex);	
		}
	}

	//get int index of which material/texture to use
	matInd = in_matInd;

/////////////////////////////////////////

    shape = new btCylinderShape(btVector3(2.5,3,2.5));

    btQuaternion rotation;
    rotation.setEulerZYX(0,0,0);
    
    //2
    btVector3 position = startPos;

    shapeMotionState = new btDefaultMotionState( btTransform(rotation, position) );     

    btScalar mass(in_mass);

    btVector3 inertia;

    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);

    rigidBody = new btRigidBody(shapeRigidBodyCI);

	rigidBody->setCcdMotionThreshold(1e-7);
	rigidBody->setCcdSweptSphereRadius(0.50);
//////////////////////////////////////

	//default init
	Init(mesh, mass, startPos);

}

void Object::createCube(aiMesh *mesh, unsigned int in_matInd, btScalar in_mass, btVector3 startPos, std::string meshType, glm::vec4 cubeSize){

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
	matInd = in_matInd;

/////////////////////////////////////////

    shape = new btBoxShape(btVector3(cubeSize.x, cubeSize.y, cubeSize.z));

    btQuaternion rotation;

    //rotation.setEulerZYX(0,0,0);
    rotation.setRotation(btVector3(0,1,0), cubeSize.w);

    //2
    btVector3 position = startPos;

    shapeMotionState = new btDefaultMotionState( btTransform(rotation, position) );     

    btScalar mass(in_mass);

    btVector3 inertia;

    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);

    rigidBody = new btRigidBody(shapeRigidBodyCI);

	rigidBody->setCcdMotionThreshold(1e-7);
	rigidBody->setCcdSweptSphereRadius(0.50);

//////////////////////////////////////

	//default init
	Init(mesh, mass, startPos);

}

void Object::createPlane(aiMesh *mesh, unsigned int in_matInd, btScalar in_mass, btVector3 startPos, std::string meshType){


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
	matInd = in_matInd;

/////////////////////////////////////////

    btCollisionShape *shape = new btStaticPlaneShape(btVector3(0,1,0), 1);

    btQuaternion rotation;
    rotation.setEulerZYX(0,0,0);
    
    //2
    btVector3 position = startPos;

    shapeMotionState = new btDefaultMotionState( btTransform(rotation, position) );     

    btScalar mass(in_mass);

    btVector3 inertia;

    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);

    rigidBody = new btRigidBody(shapeRigidBodyCI);

	rigidBody->setCcdMotionThreshold(1e-7);
	rigidBody->setCcdSweptSphereRadius(0.50);

//////////////////////////////////////

	//default init
	Init(mesh, mass, startPos);

}


////////////////////////////////////////////////////////////////////////////////////////

void Object::createComplex(aiMesh *mesh, unsigned int in_matInd, btScalar in_mass, btVector3 startPos, std::string meshType){

//temp vec3's to convert from assimp vec3's
	glm::vec3 tempVec;
	glm::vec3 tempColor;

	btVector3 triArray[3];

	btTriangleMesh *objTriMesh = new btTriangleMesh();

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

			triArray[j] = btVector3(pos.x, pos.y, pos.z);

			objTriMesh->addTriangle(triArray[0], triArray[1], triArray[2]);

			//convert to glvec3
			tempColor = {1.0f, 1.0f, 1.0f};
			tempVec = {pos.x, pos.y, pos.z};

			Vertex tempVertex = {tempVec, tempColor};
			//push_back onto Vertices each vertex
			Vertices.push_back(tempVertex);	
		}
	}

	//get int index of which material/texture to use
	matInd = in_matInd;

/////////////////////////////////////////

    btCollisionShape *shape = new btBvhTriangleMeshShape(objTriMesh, true);

    btQuaternion rotation;
    rotation.setEulerZYX(0,0,0);
    
    //2
    btVector3 position = startPos;

    shapeMotionState = new btDefaultMotionState( btTransform(rotation, position) );     

    btScalar mass(in_mass);

    btVector3 inertia;

    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);

    rigidBody = new btRigidBody(shapeRigidBodyCI);

	rigidBody->setCcdMotionThreshold(1e-7);
	rigidBody->setCcdSweptSphereRadius(.02f);

//////////////////////////////////////

	//default init
	Init(mesh, mass, startPos);

}



btRigidBody * Object::getRigidBody(){

	return rigidBody;

}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::loadDefaultCube(){


}

void Object::setDir(bool newState){
}

bool Object::getDir(){
}

bool Object::getSpin(){
}


void Object::Update(unsigned int dt, float simSpeed, float rotSim)
{

if(name.compare("Ceiling") != 0){

	btTransform trans;
	btScalar m[16]; 

	rigidBody->getMotionState()->getWorldTransform(trans);

	trans.getOpenGLMatrix(m); 

	model = glm::make_mat4(m);

	locVector.x = trans.getOrigin().getX();
	locVector.y = trans.getOrigin().getY();
	locVector.z = trans.getOrigin().getZ();

	if(name.compare("Right_Paddle") == 0){

		if(flip == true){
			if(!(currSpinAngle + dt * M_PI/100 > 3)){
  				currSpinAngle += dt * M_PI/100;
			}else{
				currSpinAngle = 3;
			}
		}else if(flip == false){
			if(!(currSpinAngle - dt * M_PI/100 < .8)){
				currSpinAngle -= dt * M_PI/100;
			}else{
				currSpinAngle = .8;	
			}
		}


		btVector3 temp = trans.getOrigin();
		btVector3 orig = temp;
		temp.setX(temp.getX() + 1);
		temp.setZ(temp.getZ() + 1);

		trans.setOrigin(temp);
    		btQuaternion tempQ;
		tempQ.setEulerZYX(0,currSpinAngle,0);
		trans.setRotation(tempQ);
		trans.setOrigin(orig);
		rigidBody->getMotionState()->setWorldTransform(trans);
		rigidBody->setMotionState(rigidBody->getMotionState());
		
	}else if(name.compare("Left_Paddle") == 0){ 

		if(flip == true){
			if(!(currSpinAngle - dt * M_PI/100 < -3)){
  				currSpinAngle -= dt * M_PI/100;
			}else{
				currSpinAngle = -3;
			}
		}else if(flip == false){
			if(!(currSpinAngle + dt * M_PI/100 > -.8)){
				currSpinAngle += dt * M_PI/100;
			}else{
				currSpinAngle = -.8;	
			}
		}

		btVector3 temp = trans.getOrigin();
		btVector3 orig = temp;
		temp.setX(temp.getX() + 1);
		temp.setZ(temp.getZ() + 1);

		trans.setOrigin(temp);
    		btQuaternion tempQ;
		tempQ.setEulerZYX(0,currSpinAngle,0);
		trans.setRotation(tempQ);
		trans.setOrigin(orig);
		rigidBody->getMotionState()->setWorldTransform(trans);
		rigidBody->setMotionState(rigidBody->getMotionState());		

	}else if(name.compare("Cube") == 0){
		setForce(btVector3(-.1,0,0));

		if(CubeTestBumperCollision()){
			score += 100;
			std::cout << "||** Current Score: " << score << " **||" << std::endl;
		}

		TestPaddles();

		if(locVector.x < -30){
			btVector3 temp;
			temp.setX(-15);
			temp.setY(-10);
			temp.setZ(25);
			trans.setOrigin(temp);
			lives -= 1;

			rigidBody->getMotionState()->setWorldTransform(trans);
			rigidBody->setMotionState(rigidBody->getMotionState());
		//-21, 20
		}
	}

}

}

bool Object::CubeTestBumperCollision(){

	glm::vec3 bump1 = m_graphics->getObject("Bumper1")->getLocVector();
	glm::vec3 bump2 = m_graphics->getObject("Bumper2")->getLocVector();
	glm::vec3 bump3 = m_graphics->getObject("Bumper3")->getLocVector();


	if(abs(locVector.x - bump1.x) < 3.75 && abs(locVector.z - bump1.z) < 3.75 && bumper1Touched == false){
		bumper1Touched = true;
		setForce(btVector3(.1,0,0));
		return true;
	}else { if(abs(locVector.x - bump1.x) > 3.75 || abs(locVector.z - bump1.z) > 3.75){
		bumper1Touched = false;
	}};

	if(abs(locVector.x - bump2.x) < 3.75 && abs(locVector.z - bump2.z) < 3.75 && bumper2Touched == false){
		bumper2Touched = true;
		return true;
	}else { if(abs(locVector.x - bump2.x) > 3.75 || abs(locVector.z - bump2.z) > 3.75){
		bumper2Touched = false;
	}};

	if(abs(locVector.x - bump3.x) < 3.75 && abs(locVector.z - bump3.z) < 3.75 && bumper3Touched == false){
		bumper3Touched = true;
		return true;
	}else { if(abs(locVector.x - bump3.x) > 3.75 || abs(locVector.z - bump3.z) > 3.75){
		bumper3Touched = false;
	}};

	return false;
}

void Object::TestPaddles(){

	glm::vec3 rPaddle = m_graphics->getObject("Right_Paddle")->getLocVector();
	bool rPaddleFlipped = m_graphics->getObject("Right_Paddle")->flip;

	if(abs(locVector.x - rPaddle.x) < 4 && abs(locVector.z - rPaddle.z) < 5
		&& rPaddleFlipped == true && rightPaddle == true){
		setForce(btVector3(9,0,0));
		rightPaddle = false;	
	}else if(abs(locVector.x - rPaddle.x) > 4 || abs(locVector.z - rPaddle.z) > 5
		|| rPaddleFlipped == false){
		rightPaddle = true;
	};

	glm::vec3 lPaddle = m_graphics->getObject("Left_Paddle")->getLocVector();
	bool lPaddleFlipped = m_graphics->getObject("Left_Paddle")->flip;

	if(abs(locVector.x - lPaddle.x) < 4 && abs(locVector.z - lPaddle.z) < 5
		&& lPaddleFlipped == true && leftPaddle == true){
		setForce(btVector3(9,0,0));
		leftPaddle = false;	
	}else if(abs(locVector.x - lPaddle.x) > 4 || abs(locVector.z - lPaddle.z) > 5
		|| lPaddleFlipped == false){
		leftPaddle = true;
	};


}

//if it has a parent object
void Object::Update(unsigned int dt, Object * parent, float simSpeed, float rotSim)
{


	//update spin angles depending on boolean
  	currSpinAngle += dt * M_PI/(spinSpeed * -1 * simSpeed);
  	currRotAngle += dt * M_PI/(rotSpeed * -1 * rotSim);

	model = glm::translate(glm::mat4(1.0f), parent->getLocVector());
	model = glm::rotate(model, (currRotAngle), glm::vec3(0,1,0));
	model = glm::translate(model, glm::vec3(0,0,radius));
	model = glm::rotate(model, (-currRotAngle), glm::vec3(0,1,0));
	model = glm::rotate(model, (currSpinAngle), glm::vec3(0,1,0));

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
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));

  //set active texture to by tex unit 0
  glActiveTexture(GL_TEXTURE0);

  //bind active texture to be texture saved by opengl at material index
  glBindTexture(GL_TEXTURE_2D, matInd);
  
  //bind texture coord buffer for shader
  glBindBuffer(GL_ARRAY_BUFFER, TB);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);

}
