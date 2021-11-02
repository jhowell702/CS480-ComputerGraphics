#version 330
          
          in vec2 texCoord;
	  out vec4 frag_color;

	  uniform sampler2D texture_sampler;
          
          void main(void)
          {
             frag_color = texture(texture_sampler, texCoord);
          }
