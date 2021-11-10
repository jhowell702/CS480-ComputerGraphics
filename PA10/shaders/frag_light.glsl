#version 330
          
      in vec2 texCoord;


	  in vec3 fN;
	  in vec3 fE;
	  in vec3 fL;

	  in mat4 modelView;

	  uniform sampler2D texture_sampler;

	  uniform vec4 ambientProduct;
	  uniform vec4 diffuseProduct;
	  uniform vec4 specularProduct;
	  uniform vec4 lightLocation;
	  uniform float shininess;

	  uniform vec4 lightLocation2;
	  uniform vec4 diffuseProduct2;
          
          void main(void)
          {
    	    vec3 N = normalize(fN); 
    	    vec3 E = normalize(fE);
    	    vec3 L = normalize(fL);

    	    vec3 H = normalize( L + E );    

    	    vec4 ambient = ambientProduct;

	    float Kd = max(dot(L, N), 0.0);
 	    vec4 diffuse = Kd*diffuseProduct;
	
 	    float Ks = pow(max(dot(N, H), 0.0), shininess);
 	    vec4 specular = Ks * specularProduct;

 	    // discard the specular highlight if the light's behind the vertex
	    if( dot(L, N) < 0.0 ) specular = vec4(0.0, 0.0, 0.0, 1.0);
   
	    vec4 color = ambient + diffuse + specular;
	    color.a = 1.0;

            gl_FragColor = texture(texture_sampler, texCoord) * color;
          }
