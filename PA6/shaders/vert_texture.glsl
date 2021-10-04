#version 330

	  layout (location = 0) in vec3 v_position;
          layout (location = 1) in vec3 v_color;
	  layout (location = 2) in vec2 v_texCoord;

          out vec2 texCoord;
          
          uniform mat4 projectionMatrix;
          uniform mat4 viewMatrix;
          uniform mat4 modelMatrix;
          
          void main(void)
          {
            vec4 v = vec4(v_position, 1.0);
            gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v;

	    texCoord = v_texCoord;			
          }
