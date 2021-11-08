#version 330
          
          in vec4 color;
	  in vec2 texCoord;

	  uniform sampler2D texture_sampler;

	  void main() 
	  {
    		gl_FragColor = texture(texture_sampler, texCoord) * color;
	  } 
