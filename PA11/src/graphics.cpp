#include "graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height, std::string* fileNames,     btDiscreteDynamicsWorld *dynamicsWorld)
{
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }


  //load all meshes from .obj
  LoadFromConfig(fileNames);
  for (auto const& x : m_objects)
  {

	dynamicsWorld->addRigidBody(x.second->getRigidBody(), 1, 1);
	x.second->dynamicsWorld = dynamicsWorld;
   }

   this->dynamicsWorld = dynamicsWorld;

   m_objects["Cube"]->getRigidBody()->setFriction(.05);

   ambientVec = glm::vec4(.15,.15,.15,1);
   diffuseVec = glm::vec4(1,0,0,1);
   specularVec = glm::vec4(.6,.6,.6,1);
   lightPos = glm::vec4(-100,100,100,1);

   diffuseVec2 = glm::vec4(0,0,1,1);
   lightPos2 = glm::vec4(500,100,-350,1);

   shininess = 1.0f;

   shaderSwitch = 0;

   spinSimSpeed = 1500.0f;
   rPSimSpeed = 10000.0f;
   rMSimSpeed = 1000.0f;

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER, fileNames[0]))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, fileNames[1]))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader2(GL_VERTEX_SHADER, fileNames[0]))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader2(GL_FRAGMENT_SHADER, fileNames[1]))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  SetUniforms();

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

bool Graphics::SetUniforms()
{

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix", shaderSwitch);
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix", shaderSwitch);
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix", shaderSwitch);
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  m_ambientProduct = m_shader->GetUniformLocation("ambientProduct", shaderSwitch);
  if (m_ambientProduct == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_ambientProduct not found\n");
    return false;
  }

  m_diffuseProduct = m_shader->GetUniformLocation("diffuseProduct", shaderSwitch);
  if (m_ambientProduct == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_diffuseProduct not found\n");
    return false;
  }

  m_specularProduct = m_shader->GetUniformLocation("specularProduct", shaderSwitch);
  if (m_ambientProduct == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_specularProduct not found\n");
    return false;
  }

  m_lightPos = m_shader->GetUniformLocation("lightLocation", shaderSwitch);
  if (m_ambientProduct == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_lightLocation not found\n");
    return false;
  }

  m_lightPos2 = m_shader->GetUniformLocation("lightLocation2", shaderSwitch);
  if (m_ambientProduct == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_lightLocation2 not found\n");
    return false;
  }

  m_diffuseProduct2 = m_shader->GetUniformLocation("diffuseProduct2", shaderSwitch);
  if (m_ambientProduct == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_diffuseProduct2 not found\n");
    return false;
  }

  m_shininess = m_shader->GetUniformLocation("shininess", shaderSwitch);
  if (m_shininess == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_shininess not found\n");
    return false;
  }
}

void Graphics::Update(unsigned int dt)
{

	m_camera->Update( m_objects[m_camera->getFocus()]->getLocVector() );


	for (auto const& x : m_objects)
	{

		if(x.second->getParent().compare("none") == 0){
			x.second->Update(dt, spinSimSpeed, rPSimSpeed);
		}else{
			x.second->Update(dt,  m_objects[ x.second->getParent() ], spinSimSpeed,rMSimSpeed);
		}
	};


//	for (Object * n : l_objects){
//		n->Update(dt, 3);
//	}

}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.67, 0.83, 0.9, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable(shaderSwitch);

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  glUniform4fv(m_ambientProduct, 1, glm::value_ptr(ambientVec));
  glUniform4fv(m_diffuseProduct, 1, glm::value_ptr(diffuseVec));
  glUniform4fv(m_specularProduct, 1, glm::value_ptr(specularVec));

  glUniform4fv(m_lightPos, 1, glm::value_ptr(lightPos));

  glUniform1f(m_shininess, shininess);

  glUniform4fv(m_diffuseProduct2, 1, glm::value_ptr(diffuseVec2));
  glUniform4fv(m_lightPos2, 1, glm::value_ptr(lightPos2));

  // Render the object
//  for(Object * model : l_objects){
//	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(model->GetModel()));
//	model->RenderTextures();
//  }

	for (auto const& x : m_objects)
	{
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(x.second->GetModel()));
		x.second->RenderTextures();
	};


  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

void Graphics::LoadFromConfig(std::string* fileNames){

	std::ifstream file("config");

	std::string tester;
	std::string name;
	float scale;
	std::string parent;
	std::string objFileName;
	btScalar mass;
	btScalar posX;
	btScalar posY;
	btScalar posZ;
	std::string meshType;
	glm::vec4 cubeSize;

	while(!file.eof()){
	
		file >> tester;

		if(tester.compare("Object") == 0){
			file >> name;
			file >> tester >> mass;
			file >> tester >> posX;
			file >> tester >> posY;
			file >> tester >> posZ;
			btVector3 startPos(posX, posY, posZ);
			file >> tester >> scale;
			file >> tester >> meshType;
			if(meshType.compare("cube") == 0){
				file >> cubeSize.x >> cubeSize.y >> cubeSize.z >> cubeSize.w;
			};
			file >> tester >> parent;
			file >> tester >> objFileName;

			Object * temp = LoadObjects(objFileName, mass, startPos, meshType, cubeSize);
			temp->setName(name);
			temp->setScale(scale);
			temp->setParent(parent);
			temp->setGraphics(this);
	
			m_objects.insert(pair<std::string,Object *>(name, temp));			


		}
	}	

}

///////////////////////////////////////////////////////////

Object * Graphics::LoadObjects(std::string fileName, btScalar mass, btVector3 startPos, std::string meshType, glm::vec4 cubeSize){

	objName = fileName;

	unsigned int * matInd;

	//load scene from obj file
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile("../models/" + fileName, 		aiProcess_Triangulate);

	//get number of meshes in obj
	int numMeshes = scene->mNumMeshes;

	//get number of materials in obj
	int numMaterials = scene->mNumMaterials;
	
	//for each material
	for(int i = 0; i < numMaterials; i++){
		aiMaterial * test = scene->mMaterials[i];
		
		//get texture file path
		aiString Path;
        	if (test->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
		
		//concatenate texture name and path to folder with textures
                std::string FullPath = "../models/";
		FullPath = FullPath + Path.data;
	
		//load textures using file name and path
		matInd = loadTextures(FullPath);
		}
	}

	//for each mesh
	//for(int i = 0; i < numMeshes; i++){
			//create a new Object
			Object * temp = new Object(scene->mMeshes[0], *matInd, mass, startPos, meshType, cubeSize);
			//push back onto list of objects
			return temp;
	//}



}

unsigned int * Graphics::loadTextures(std::string texFileName){

	int width, height, nrChannels;
	//using stbi, load texture file

	stbi_set_flip_vertically_on_load(1);
	unsigned char *data = stbi_load(texFileName.c_str(), &width, &height, &nrChannels, 0); 

	//material/texture index, to be called during render
	unsigned int * texture = new unsigned int;

	//generate and attached texture pointer
	glGenTextures(1, texture);

	//bind texture to index saved by texture
	glBindTexture(GL_TEXTURE_2D, *texture); 

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//giving tetxture to opengl
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	//generate mipmap for this texture
	glGenerateMipmap(GL_TEXTURE_2D);

	//free stbi image data
	stbi_image_free(data);

	//return texture/material index
	return texture;
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

