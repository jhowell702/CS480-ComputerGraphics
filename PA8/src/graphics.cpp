#include "graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height, std::string* fileNames)
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

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
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
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

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
	float rot;
	float spin;
	float radius;
	float scale;
	std::string parent;
	std::string objFileName;


	while(!file.eof()){
	
		file >> tester;

		if(tester.compare("Object") == 0){
			file >> name;
			file >> tester >> spin;
			file >> tester >> rot;
			file >> tester >> radius;
			file >> tester >> scale;
			file >> tester >> parent;
			file >> tester >> objFileName;

			Object * temp = LoadObjects(objFileName);
			temp->setName(name);
			temp->setSpinSpeed(spin);
			temp->setRotSpeed(rot);
			temp->setScale(scale);
			temp->setRadius(radius);
			temp->setParent(parent);

	
			m_objects.insert(pair<std::string,Object *>(name, temp));			


		}
	}	

}

///////////////////////////////////////////////////////////

Object * Graphics::LoadObjects(std::string fileName){

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
			Object * temp = new Object(scene->mMeshes[0], *matInd);
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

