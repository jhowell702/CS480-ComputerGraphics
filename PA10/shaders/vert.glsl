#version 330

	  layout (location = 0) in vec3 v_position;
          layout (location = 1) in vec3 v_normals;
	  layout (location = 2) in vec2 v_texCoord;

          out vec4 color;
	  out vec2 texCoord;
          
          uniform mat4 projectionMatrix;
          uniform mat4 viewMatrix;
          uniform mat4 modelMatrix;

	  	  uniform vec4 ambientProduct;
	  	  uniform vec4 diffuseProduct;
	  	  uniform vec4 specularProduct;
	  	  uniform vec4 lightLocation;
	  	  uniform float shininess;
          
          void main(void)
          {
            vec4 v = vec4(v_position, 1.0);
	    mat4 modelView = viewMatrix * modelMatrix;
            gl_Position = (projectionMatrix * modelView) * v;

    		// Transform vertex  position into eye coordinates
   		vec3 pos = (modelView * v).xyz;

    		vec3 L = normalize( lightLocation.xyz - pos );
    		vec3 E = normalize( -pos );
    		vec3 H = normalize( L + E );

		    // Transform vertex normal into eye coordinates
		    vec3 N = normalize( modelView * vec4(v_normals, 0.0) ).xyz;

    		vec4 ambient = ambientProduct;
    		float Kd = max( dot(L, N), 0.0 );
    		vec4  diffuse = Kd * diffuseProduct;
    		float Ks = pow( max(dot(N, H), 0.0), shininess );

    		vec4  specular = Ks * specularProduct;

    		if( dot(L, N) < 0.0 )  specular = vec4(0.0, 0.0, 0.0, 1.0);

		    color = ambient + diffuse + specular;
		    color.a = 1.0;

	    	texCoord = v_texCoord;			
          }
