#version 330

	  layout (location = 0) in vec3 v_position;
          layout (location = 1) in vec3 v_normals;
	  layout (location = 2) in vec2 v_texCoord;

          out vec2 texCoord;
	  out vec3 fN;
	  out vec3 fE;
	  out vec3 fL;

	  out mat4 modelView;
          
          uniform mat4 projectionMatrix;
          uniform mat4 viewMatrix;
          uniform mat4 modelMatrix;

	  uniform vec4 lightLocation;
          
          void main(void)
          {
            vec4 v = vec4(v_position, 1.0);
            gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v;
	    modelView = viewMatrix * modelMatrix;

            vec3 pos = (modelView * v).xyz;

    	    fN = v_normals;
	    fE = (modelView * vec4(v_position, 1.0)).xyz;
    	    fL = lightLocation.xyz - v_position.xyz;


	    texCoord = v_texCoord;			
          }
