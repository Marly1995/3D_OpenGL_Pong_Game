// tag::C++11check[]
#define STRING2(x) #x
#define STRING(x) STRING2(x)

#if __cplusplus < 201103L
	#pragma message("WARNING: the compiler may not be C++11 compliant. __cplusplus version is : " STRING(__cplusplus))
#endif
// end::C++11check[]

// tag::includes[]
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>


#include <GL/glew.h>
#include <SDL.h>

#define GLM_FORCE_RADIANS // suppress a warning in GLM 0.9.5
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
// end::includes[]

// tag::using[]
// see https://isocpp.org/wiki/faq/Coding-standards#using-namespace-std
// don't use the whole namespace, either use the specific ones you want, or just type std::
using std::cout;
using std::cerr;
using std::endl;
using std::max;
using std::string;
// end::using[]


// tag::globalVariables[]
std::string exeName;
SDL_Window *win; //pointer to the SDL_Window
SDL_GLContext context; //the SDL_GLContext
int frameCount = 0;
std::string frameLine = "";
// end::globalVariables[]

// tag::loadShader[]
std::string loadShader(const string filePath) {
    std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);
	if (fileStream)
	{
		string fileData( (std::istreambuf_iterator<char>(fileStream)),
		                 (std::istreambuf_iterator<char>()          ));

		cout << "Shader Loaded from " << filePath << endl;
		return fileData;
	}
	else
	{
        cerr << "Shader could not be loaded - cannot read file " << filePath << ". File does not exist." << endl;
        return "";
	}
}
// end::loadShader[]

//our variables
bool done = false;

// tag::vertexData[]
//the data about our geometry
const GLfloat LeftvertexData[] = {
	// X	Y	VERTEX	  X	   Y	TEXTURE
	-0.05f, -0.2f, -0.05f,
	 0.05f, -0.2f, -0.05f,
	 0.05f,  0.2f, -0.05f,
	 0.05f,  0.2f, -0.05f,
	-0.05f,  0.2f, -0.05f,
	-0.05f, -0.2f, -0.05f,

	-0.05f, -0.2f,  0.05f,
	 0.05f, -0.2f,  0.05f,
	 0.05f,  0.2f,  0.05f,
	 0.05f,  0.2f,  0.05f,
	-0.05f,  0.2f,  0.05f,
	-0.05f, -0.2f,  0.05f,

	-0.05f,  0.2f,  0.05f,
	-0.05f,  0.2f, -0.05f,
	-0.05f, -0.2f, -0.05f,
	-0.05f, -0.2f, -0.05f,
	-0.05f, -0.2f,  0.05f,
	-0.05f,  0.2f,  0.05f,

	 0.05f,  0.2f,  0.05f,
	 0.05f,  0.2f, -0.05f,
	 0.05f, -0.2f, -0.05f,
	 0.05f, -0.2f, -0.05f,
	 0.05f, -0.2f,  0.05f,
	 0.05f,  0.2f,  0.05f,

	-0.05f, -0.2f, -0.05f,
	 0.05f, -0.2f, -0.05f,
	 0.05f, -0.2f,  0.05f,
	 0.05f, -0.2f,  0.05f,
	-0.05f, -0.2f,  0.05f,
	-0.05f, -0.2f, -0.05f,

	-0.05f,  0.2f, -0.05f,
	 0.05f,  0.2f, -0.05f,
	 0.05f,  0.2f,  0.05f,
	 0.05f,  0.2f,  0.05f,
	-0.05f,  0.2f,  0.05f,
	-0.05f,  0.2f, -0.05f
};
// Right Paddle vertex data
const GLfloat RightvertexData[] = {
	// X	Y	VERTEX	  X	   Y	TEXTURE
	-0.05f, -0.2f, -0.05f,
	 0.05f, -0.2f, -0.05f,
	 0.05f,  0.2f, -0.05f,
	 0.05f,  0.2f, -0.05f,
	-0.05f,  0.2f, -0.05f,
	-0.05f, -0.2f, -0.05f,

	-0.05f, -0.2f,  0.05f,
	 0.05f, -0.2f,  0.05f,
	 0.05f,  0.2f,  0.05f,
	 0.05f,  0.2f,  0.05f,
	-0.05f,  0.2f,  0.05f,
	-0.05f, -0.2f,  0.05f,

	-0.05f,  0.2f,  0.05f,
	-0.05f,  0.2f, -0.05f,
	-0.05f, -0.2f, -0.05f,
	-0.05f, -0.2f, -0.05f,
	-0.05f, -0.2f,  0.05f,
	-0.05f,  0.2f,  0.05f,

	 0.05f,  0.2f,  0.05f,
	 0.05f,  0.2f, -0.05f,
	 0.05f, -0.2f, -0.05f,
	 0.05f, -0.2f, -0.05f,
	 0.05f, -0.2f,  0.05f,
	 0.05f,  0.2f,  0.05f,

	-0.05f, -0.2f, -0.05f,
	 0.05f, -0.2f, -0.05f,
	 0.05f, -0.2f,  0.05f,
	 0.05f, -0.2f,  0.05f,
	-0.05f, -0.2f,  0.05f,
	-0.05f, -0.2f, -0.05f,

	-0.05f,  0.2f, -0.05f,
	 0.05f,  0.2f, -0.05f,
	 0.05f,  0.2f,  0.05f,
	 0.05f,  0.2f,  0.05f,
	-0.05f,  0.2f,  0.05f,
	-0.05f,  0.2f, -0.05f
};
// Boundarys vertex data
const GLfloat boundsVertexData[] = {
	// X	Y	VERTEX		  // top
	-1.0f, -1.0f, -0.05f,
	-0.9f, -1.0f, -0.05f,
	-0.9f,  1.0f, -0.05f,
	-0.9f,  1.0f, -0.05f,
	-1.0f,  1.0f, -0.05f,
	-1.0f, -1.0f, -0.05f,

	-1.0f, -1.0f,  0.05f,
	-0.9f, -1.0f,  0.05f,
	-0.9f,  1.0f,  0.05f,
	-0.9f,  1.0f,  0.05f,
	-1.0f,  1.0f,  0.05f,
	-1.0f, -1.0f,  0.05f,

	-1.0f,  1.0f,  0.05f,
	-1.0f,  1.0f, -0.05f,
	-1.0f, -1.0f, -0.05f,
	-1.0f, -1.0f, -0.05f,
	-1.0f, -1.0f,  0.05f,
	-1.0f,  1.0f,  0.05f,

	-0.9f,  1.0f,  0.05f,
	-0.9f,  1.0f, -0.05f,
	-0.9f, -1.0f, -0.05f,
	-0.9f, -1.0f, -0.05f,
	-0.9f, -1.0f,  0.05f,
	-0.9f,  1.0f,  0.05f,

	-1.0f, -1.0f, -0.05f,
	-0.9f, -1.0f, -0.05f,
	-0.9f, -1.0f,  0.05f,
	-0.9f, -1.0f,  0.05f,
	-1.0f, -1.0f,  0.05f,
	-1.0f, -1.0f, -0.05f,

	-1.0f,  1.0f, -0.05f,
	-0.9f,  1.0f, -0.05f,
	-0.9f,  1.0f,  0.05f,
	-0.9f,  1.0f,  0.05f,
	-1.0f,  1.0f,  0.05f,
	-1.0f,  1.0f, -0.05f,
	//left
	
	1.0f, -1.0f, -0.05f,
	0.9f, -1.0f, -0.05f,
	0.9f,  1.0f, -0.05f,
	0.9f,  1.0f, -0.05f,
	1.0f,  1.0f, -0.05f,
	1.0f, -1.0f, -0.05f,

	1.0f, -1.0f,  0.05f,
	0.9f, -1.0f,  0.05f,
	0.9f,  1.0f,  0.05f,
	0.9f,  1.0f,  0.05f,
	1.0f,  1.0f,  0.05f,
	1.0f, -1.0f,  0.05f,

	1.0f,  1.0f,  0.05f,
	1.0f,  1.0f, -0.05f,
	1.0f, -1.0f, -0.05f,
	1.0f, -1.0f, -0.05f,
	1.0f, -1.0f,  0.05f,
	1.0f,  1.0f,  0.05f,

	0.9f,  1.0f,  0.05f,
	0.9f,  1.0f, -0.05f,
	0.9f, -1.0f, -0.05f,
	0.9f, -1.0f, -0.05f,
	0.9f, -1.0f,  0.05f,
	0.9f,  1.0f,  0.05f,

	1.0f, -1.0f, -0.05f,
	0.9f, -1.0f, -0.05f,
	0.9f, -1.0f,  0.05f,
	0.9f, -1.0f,  0.05f,
	1.0f, -1.0f,  0.05f,
	1.0f, -1.0f, -0.05f,

	1.0f,  1.0f, -0.05f,
	0.9f,  1.0f, -0.05f,
	0.9f,  1.0f,  0.05f,
	0.9f,  1.0f,  0.05f,
	1.0f,  1.0f,  0.05f,
	1.0f,  1.0f, -0.05f,
	/////////////
	///////////////

	 1.0f,  0.9f, -0.05f,
    -1.0f,  0.9f, -0.05f,
	-1.0f,  1.0f, -0.05f,
	-1.0f,  1.0f, -0.05f,
	 1.0f,  1.0f, -0.05f,
	 1.0f,  0.9f, -0.05f,
		   
	 1.0f,  0.9f,  0.05f,
	-1.0f,  0.9f,  0.05f,
	-1.0f,  1.0f,  0.05f,
	-1.0f,  1.0f,  0.05f,
	 1.0f,  1.0f,  0.05f,
	 1.0f,  0.9f,  0.05f,
		   
	 1.0f,  1.0f,  0.05f,
	 1.0f,  1.0f, -0.05f,
	 1.0f,  0.9f, -0.05f,
	 1.0f,  0.9f, -0.05f,
	 1.0f,  0.9f,  0.05f,
	 1.0f,  1.0f,  0.05f,
		   
	-1.0f,  1.0f,  0.05f,
	-1.0f,  1.0f, -0.05f,
	-1.0f,  0.9f, -0.05f,
	-1.0f,  0.9f, -0.05f,
	-1.0f,  0.9f,  0.05f,
	-1.0f,  1.0f,  0.05f,
	  
	 1.0f,  0.9f, -0.05f,
	-1.0f,  0.9f, -0.05f,
	-1.0f,  0.9f,  0.05f,
	-1.0f,  0.9f,  0.05f,
	 1.0f,  0.9f,  0.05f,
	 1.0f,  0.9f, -0.05f,
		   
	 1.0f,  1.0f, -0.05f,
	-1.0f,  1.0f, -0.05f,
	-1.0f,  1.0f,  0.05f,
	-1.0f,  1.0f,  0.05f,
	 1.0f,  1.0f,  0.05f,
	 1.0f,  1.0f, -0.05f,
		/////////////
		///////////////

 	 1.0f, -0.9f, -0.05f,
	-1.0f, -0.9f, -0.05f,
	-1.0f, -1.0f, -0.05f,
	-1.0f, -1.0f, -0.05f,
	 1.0f, -1.0f, -0.05f,
	 1.0f, -0.9f, -0.05f,

	 1.0f, -0.9f, 0.05f,
	-1.0f, -0.9f, 0.05f,
	-1.0f, -1.0f, 0.05f,
	-1.0f, -1.0f, 0.05f,
	 1.0f, -1.0f, 0.05f,
	 1.0f, -0.9f, 0.05f,

	 1.0f, -1.0f, 0.05f,
	 1.0f, -1.0f, -0.05f,
	 1.0f, -0.9f, -0.05f,
	 1.0f, -0.9f, -0.05f,
	 1.0f, -0.9f, 0.05f,
	 1.0f, -1.0f, 0.05f,

	-1.0f, -1.0f, 0.05f,
	-1.0f, -1.0f, -0.05f,
	-1.0f, -0.9f, -0.05f,
	-1.0f, -0.9f, -0.05f,
	-1.0f, -0.9f, 0.05f,
	-1.0f, -1.0f, 0.05f,

	 1.0f, -0.9f, -0.05f,
	-1.0f, -0.9f, -0.05f,
	-1.0f, -0.9f, 0.05f,
	-1.0f, -0.9f, 0.05f,
	 1.0f, -0.9f, 0.05f,
	 1.0f, -0.9f, -0.05f,

	 1.0f, -1.0f, -0.05f,
	-1.0f, -1.0f, -0.05f,
	-1.0f, -1.0f, 0.05f,
	-1.0f, -1.0f, 0.05f,
	 1.0f, -1.0f, 0.05f,
	 1.0f, -1.0f, -0.05f
};
// ball vertex data
GLfloat cubeVertexData[]{
	-0.1f, -0.1f, -0.1f, 
	 0.1f, -0.1f, -0.1f, 
	 0.1f,  0.1f, -0.1f, 
	 0.1f,  0.1f, -0.1f, 
	-0.1f,  0.1f, -0.1f, 
	-0.1f, -0.1f, -0.1f, 

	-0.1f, -0.1f,  0.1f, 
	 0.1f, -0.1f,  0.1f, 
	 0.1f,  0.1f,  0.1f,
	 0.1f,  0.1f,  0.1f,  
	-0.1f,  0.1f,  0.1f,
	-0.1f, -0.1f,  0.1f, 

	-0.1f,  0.1f,  0.1f, 
	-0.1f,  0.1f, -0.1f,  
	-0.1f, -0.1f, -0.1f,  
	-0.1f, -0.1f, -0.1f, 
	-0.1f, -0.1f,  0.1f,  
	-0.1f,  0.1f,  0.1f,  

	 0.1f,  0.1f,  0.1f, 
	 0.1f,  0.1f, -0.1f, 
	 0.1f, -0.1f, -0.1f, 
	 0.1f, -0.1f, -0.1f,  
	 0.1f, -0.1f,  0.1f,
	 0.1f,  0.1f,  0.1f,

	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,

	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f,
	 0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f
};

GLfloat skyboxVertexData[]{
	-10.0f, -10.0f, -10.0,		
	10.0f, -10.0f, -10.0f,
	10.0f,  10.0f, -10.0f,
	10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,
	-10.0f, -10.0f, -10.0f,

	-10.0f, -10.0f,  10.0f,
	10.0f, -10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f,  10.0f,
	-10.0f, -10.0f,  10.0f,

	-10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f, -10.0f,
	-10.0f, -10.0f, -10.0f,
	-10.0f, -10.0f, -10.0f,
	-10.0f, -10.0f,  10.0f,
	-10.0f,  10.0f,  10.0f,

	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f,  10.0f,
	10.0f,  10.0f, 10.0f,

	-10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f,  10.0f,
	10.0f, -10.0f,  10.0f,
	-10.0f, -10.0f,  10.0f,
	-10.0f, -10.0f, -10.0f,

	-10.0f,  10.0f, -10.0f,
	10.0f,  10.0f, -10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f, -10.0f
};

GLfloat cubeColorData[]{
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	1.0f,  1.0f,  1.0f

};

GLfloat cubeTextureData[]{
	0.75f, 0.666f,
	0.99f, 0.666f,
	0.99f, 0.333f,
	0.99f, 0.333f,
	0.75f, 0.333f,
	0.75f, 0.666f,

	0.25f, 0.666f,
	0.50f, 0.666f,
	0.50f, 0.333f,
	0.50f, 0.333f,
	0.25f, 0.333f,
	0.25f, 0.666f,

	0.25f, 0.333f,
	0.00f, 0.333f,
	0.00f, 0.666f,
	0.00f, 0.666f,
	0.25f, 0.666f,
	0.25f, 0.333f,

	0.49f, 0.333f,
	0.74f, 0.333f,
	0.74f, 0.666f,
	0.74f, 0.666f,
	0.49f, 0.666f,
	0.49f, 0.333f,

	0.25f, 0.666f,
	0.50f, 0.666f,
	0.50f, 1.000f,
	0.50f, 1.000f,
	0.25f, 1.000f,
	0.25f, 0.666f,

	0.25f, 0.000f,
	0.50f, 0.000f,
	0.50f, 0.333f,
	0.50f, 0.333f,
	0.25f, 0.333f,
	0.25f, 0.000f,
};

GLfloat rightUIvertexData[]{
	0.9f, 0.7f,		1.0f, 0.0f,
	0.7f, 0.9f,		0.0f, 1.0f,
	0.9f, 0.9f,		1.0f, 1.0f,

	0.7f, 0.9f,		0.0f, 1.0f,
	0.9f, 0.7f,		1.0f, 0.0f,
	0.7f, 0.7f,		0.0f, 0.0f

};
GLfloat leftUIvertexData[]{
	-0.9f, 0.7f,	1.0f, 0.0f,
	-0.7f, 0.9f,	0.0f, 1.0f,
	-0.9f, 0.9f,	1.0f, 1.0f,

	-0.7f, 0.9f,	0.0f, 1.0f,
	-0.9f, 0.7f,	1.0f, 0.0f,
	-0.7f, 0.7f,	0.0f, 0.0f

};

// end::vertexData[]

// tag::gameState[]
//the translation vector we'll pass to our GLSL program
glm::vec3 padLpos = { -0.80f, 0.00f, 0.00f};
glm::vec3 padLvel = { 0.00f, 0.00f, 0.00f};

glm::vec3 padRpos = { 0.80f, 0.00f, 0.00f};
glm::vec3 padRvel = { 0.00f, 0.00f, 0.00f};

glm::vec3 ballPos = { 0.00f, 0.00f, 0.00f };
glm::vec3 ballVel = { 0.015f, 0.01f, 0.00f };

GLfloat radius = 10.0f;
GLfloat camX = 0.0f;
GLfloat camZ = 0.0f;

glm::mat4 view;
glm::mat4 view1;
glm::mat4 view2;
glm::mat4 view3;
glm::mat4 view4;

glm::vec3 skyBoxPosition = glm::vec3(0.0f, 0.0f, 0.0f);

GLfloat cameraSpeed = 0.05f;
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, -2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 skyBoxUp;
// end::gameState[]

//Lighting jazzzzzzzzz
glm::vec3 lightPosition(0.0f, 1.0f, -1.0f);
float lightMove = 0.01f;
float lightColor[] = { 0.8f, 0.8f, 0.4f };


glm::mat4 rotateMatrix; // the transformation matrix for our object - which is the identity matrix by default
float rotateSpeed = 1.0f; //rate of change of the rotate - in radians per second

glm::mat4 padLmatrix;
glm::mat4 padRmatrix;
glm::mat4 ballMatrix;
glm::mat4 lightMatrix;
glm::mat4 skyBoxmatrix;
glm::mat4 skyBoxRotatematrix;

// tag::GLVariables[]
//our GL and GLSL variables
//programIDs
GLuint theProgram; //GLuint that we'll fill in to refer to the GLSL program (only have 1 at this point)

//attribute locations
GLint positionLocation; //GLuint that we'll fill in with the location of the `position` attribute in the GLSL
GLint vertexColorLocation; //GLuint that we'll fill in with the location of the `vertexColor` attribute in the GLSL
GLint textureLocation;

//uniform location
GLint modelMatrixLocation;
GLint viewMatrixLocation;
GLint projectionMatrixLocation;
GLint rotateMatrixLocation;
GLint lightPositionLocation;
GLint lightColorLocation;
GLint cameraPositionLocation;
GLint textureBool;

GLuint LeftPaddleVertexDataBufferObject;
GLuint LeftPaddleVertexArrayObject;
GLuint LeftPaddleTexture;

GLuint RightPaddleVertexDataBufferObject;
GLuint RightPaddleVertexArrayObject;
GLuint RightPaddleTexture;

GLuint boundsVertexDataBufferObject;
GLuint boundsVertexArrayObject;
GLuint boundsTexture;

GLuint cubeVertexDataBufferObject;
GLuint cubeVertexArrayObject;
//light uses cube data cos lazyness
GLuint lightVertexArrayObject;

GLuint rightUIVertexDataBufferObject;
GLuint rightUIVertexArrayObject;

GLuint leftUIVertexDataBufferObject;
GLuint leftUIVertexArrayObject;

GLuint BallVertexDataBufferObject;
GLuint BallVertexArrayObject;
GLuint ballTexture;
GLuint skyboxTex;

GLuint skyboxVertexDataBufferObject;
GLuint skyboxVertexArrayObject;

GLuint ColorDataBufferObject;
GLuint ColorArrayObject;

GLuint TextureDataBufferObject;
GLuint TextureArrayObject;

GLuint Rscore0Texture;
GLuint Rscore1Texture;
GLuint Rscore2Texture;
GLuint RloserTexture;
GLuint RwinnerTexture;

GLuint Lscore0Texture;
GLuint Lscore1Texture;
GLuint Lscore2Texture;
GLuint LloserTexture;
GLuint LwinnerTexture;

// end::GLVariables[]

bool go = false;
bool gameOver = false;

bool rpUp = false;
bool rpDown = false;
bool lpUp = false;
bool lpDown = false;

int RPscore = 0;
int LPscore = 0;

bool cameraForward = false;
bool cameraBackward = false;
bool cameraLeft = false;
bool cameraRight = false;

int mousePosition[] = { 0, 0 };
float mousePos[] = { 0.0, 0.0 };
float lastx;
float lasty;

int cameraStyle = 0;

// end Global Variables
/////////////////////////

// tag::initialise[]
void initialise()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	cout << "SDL initialised OK!\n";
}
// end::initialise[]

// tag::createWindow[]
void createWindow()
{
	//get executable name, and use as window title
	int beginIdxWindows = exeName.rfind("\\"); //find last occurrence of a backslash
	int beginIdxLinux = exeName.rfind("/"); //find last occurrence of a forward slash
	int beginIdx = max(beginIdxWindows, beginIdxLinux);
	std::string exeNameEnd = exeName.substr(beginIdx + 1);
	const char *exeNameCStr = exeNameEnd.c_str();

	//create window
	win = SDL_CreateWindow(exeNameCStr, 100, 100, 600, 600, SDL_WINDOW_OPENGL); //same height and width makes the window square ...

	//error handling
	if (win == nullptr)
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	cout << "SDL CreatedWindow OK!\n";
}
// end::createWindow[]

// tag::setGLAttributes[]
void setGLAttributes()
{
	int major = 3;
	int minor = 3;
	cout << "Built for OpenGL Version " << major << "." << minor << endl; //ahttps://en.wikipedia.org/wiki/OpenGL_Shading_Language#Versions
	// set the opengl context version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //core profile
	cout << "Set OpenGL context to versicreate remote branchon " << major << "." << minor << " OK!\n";
}
// tag::setGLAttributes[]

// tag::createContext[]
void createContext()
{
	setGLAttributes();

	context = SDL_GL_CreateContext(win);
	if (context == nullptr){
		SDL_DestroyWindow(win);
		std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	cout << "Created OpenGL context OK!\n";
}
// end::createContext[]

// tag::initGlew[]
void initGlew()
{
	GLenum rev;
	glewExperimental = GL_TRUE; //GLEW isn't perfect - see https://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
	rev = glewInit();
	if (GLEW_OK != rev){
		std::cerr << "GLEW Error: " << glewGetErrorString(rev) << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		cout << "GLEW Init OK!\n";
	}
}
// end::initGlew[]

// tag::createShader[]
GLuint createShader(GLenum eShaderType, const std::string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	//error check
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}
// end::createShader[]

// tag::createProgram[]
GLuint createProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}
// end::createProgram[]

// tag::initializeProgram[]
void initializeProgram()
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(createShader(GL_VERTEX_SHADER, loadShader("vertexShader.glsl")));
	shaderList.push_back(createShader(GL_FRAGMENT_SHADER, loadShader("fragmentShader.glsl")));

	theProgram = createProgram(shaderList);
	if (theProgram == 0)
	{
		cerr << "GLSL program creation error." << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		cout << "GLSL program creation OK! GLUint is: " << theProgram << std::endl;
	}

	// tag::glGetAttribLocation[]
	positionLocation = glGetAttribLocation(theProgram, "position");
	vertexColorLocation = glGetAttribLocation(theProgram, "vertexColor");
	textureLocation = glGetAttribLocation(theProgram, "texture");
	// end::glGetAttribLocation[]

	// tag::glGetUniformLocation[]
	modelMatrixLocation = glGetUniformLocation(theProgram, "modelMatrix");
	viewMatrixLocation = glGetUniformLocation(theProgram, "viewMatrix");
	projectionMatrixLocation = glGetUniformLocation(theProgram, "projectionMatrix");
	rotateMatrixLocation = glGetUniformLocation(theProgram, "rotateMatrix");
	lightColorLocation = glGetUniformLocation(theProgram, "lightColor");
	lightPositionLocation = glGetUniformLocation(theProgram, "lightPosition");
	cameraPositionLocation = glGetUniformLocation(theProgram, "cameraPosition");
	textureBool = glGetUniformLocation(theProgram, "texBool");

	//only generates runtime code in debug mode
	assert( modelMatrixLocation != -1);
	assert( viewMatrixLocation != -1);
	assert( projectionMatrixLocation != -1);
	// end::glGetUniformLocation[]

	//clean up shaders (we don't need them anymore as they are no in theProgram
	for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}
// end::initializeProgram[]

// tag::initializeVertexArrayObject[]
//setup a GL object (a VertexArrayObject) that stores how to access data and from where
void initializeVertexArrayObject()
{

	// LIGHTING SRTUDKFNS
	glGenVertexArrays(1, &lightVertexArrayObject); //create a Vertex Array Object
	cout << "Vertex Array Object created OK! GLUint is: " << boundsVertexArrayObject << std::endl;
	glBindVertexArray(lightVertexArrayObject); //make the just created vertexArrayObject the active one
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexDataBufferObject); //bind vertexDataBufferObjec
	glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it
	//bounds
	glGenVertexArrays(1, &boundsVertexArrayObject); //create a Vertex Array Object
	cout << "Vertex Array Object created OK! GLUint is: " << boundsVertexArrayObject << std::endl;
	glBindVertexArray(boundsVertexArrayObject); //make the just created vertexArrayObject the active one
	glBindBuffer(GL_ARRAY_BUFFER, boundsVertexDataBufferObject); //bind vertexDataBufferObject
	glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); //specify that position data contains four floats per vertex, and goes into attribute index positionLocation
	glEnableVertexAttribArray(vertexColorLocation);
	glBindBuffer(GL_ARRAY_BUFFER, ColorDataBufferObject);
	glVertexAttribPointer(vertexColorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it

	//cuuuube
	glGenVertexArrays(1, &cubeVertexArrayObject); //create a Vertex Array Object
	cout << "Vertex Array Object created OK! GLUint is: " << boundsVertexArrayObject << std::endl;
	glBindVertexArray(cubeVertexArrayObject); //make the just created vertexArrayObject the active one
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexDataBufferObject); //bind vertexDataBufferObjec
	glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, TextureDataBufferObject);
	glEnableVertexAttribArray(textureLocation);
	glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexColorLocation);
	glBindBuffer(GL_ARRAY_BUFFER, ColorDataBufferObject);
	glVertexAttribPointer(vertexColorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it

	//ui
	glGenVertexArrays(1, &rightUIVertexArrayObject); //create a Vertex Array Object
	cout << "Vertex Array Object created OK! GLUint is: " << boundsVertexArrayObject << std::endl;
	glBindVertexArray(rightUIVertexArrayObject); //make the just created vertexArrayObject the active one
	glBindBuffer(GL_ARRAY_BUFFER, rightUIVertexDataBufferObject); //bind vertexDataBufferObjec
	glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, (4 * sizeof(GL_FLOAT)), (GLvoid *)(0 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index positionLocation
	glEnableVertexAttribArray(textureLocation);
	glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, (4 * sizeof(GL_FLOAT)), (GLvoid *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(vertexColorLocation);
	glBindBuffer(GL_ARRAY_BUFFER, ColorDataBufferObject);
	glVertexAttribPointer(vertexColorLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it

	glGenVertexArrays(1, &leftUIVertexArrayObject); //create a Vertex Array Object
	cout << "Vertex Array Object created OK! GLUint is: " << boundsVertexArrayObject << std::endl;
	glBindVertexArray(leftUIVertexArrayObject); //make the just created vertexArrayObject the active one
	glBindBuffer(GL_ARRAY_BUFFER, leftUIVertexDataBufferObject); //bind vertexDataBufferObjec
	glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, (4 * sizeof(GL_FLOAT)), (GLvoid *)(0 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index positionLocation
	glEnableVertexAttribArray(textureLocation);
	glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, (4 * sizeof(GL_FLOAT)), (GLvoid *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(vertexColorLocation);
	glBindBuffer(GL_ARRAY_BUFFER, ColorDataBufferObject);
	glVertexAttribPointer(vertexColorLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it

	glGenVertexArrays(1, &skyboxVertexArrayObject); //create a Vertex Array Object
	cout << "Vertex Array Object created OK! GLUint is: " << skyboxVertexArrayObject << std::endl;
	glBindVertexArray(skyboxVertexArrayObject); //make the just created vertexArrayObject the active one
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVertexDataBufferObject); //bind vertexDataBufferObject
	glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, TextureDataBufferObject);
	glEnableVertexAttribArray(textureLocation);
	glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexColorLocation);
	glBindBuffer(GL_ARRAY_BUFFER, ColorDataBufferObject);
	glVertexAttribPointer(vertexColorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it

						  //Left Paddle
	glGenVertexArrays(1, &LeftPaddleVertexArrayObject); //create a Vertex Array Object
	cout << "Vertex Array Object created OK! GLUint is: " << LeftPaddleVertexArrayObject << std::endl;
	glBindVertexArray(LeftPaddleVertexArrayObject); //make the just created vertexArrayObject the active one
	glBindBuffer(GL_ARRAY_BUFFER, LeftPaddleVertexDataBufferObject); //bind vertexDataBufferObject
	glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, TextureDataBufferObject);
	glEnableVertexAttribArray(textureLocation);
	glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexColorLocation);
	glBindBuffer(GL_ARRAY_BUFFER, ColorDataBufferObject);
	glVertexAttribPointer(vertexColorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it

						  //Right Paddle
	glGenVertexArrays(1, &RightPaddleVertexArrayObject); //create a Vertex Array Object
	cout << "Vertex Array Object created OK! GLUint is: " << RightPaddleVertexArrayObject << std::endl;
	glBindVertexArray(RightPaddleVertexArrayObject); //make the just created vertexArrayObject the active one
	glBindBuffer(GL_ARRAY_BUFFER, RightPaddleVertexDataBufferObject); //bind vertexDataBufferObject
	glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexColorLocation);
	glBindBuffer(GL_ARRAY_BUFFER, ColorDataBufferObject);
	glVertexAttribPointer(vertexColorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it

	glGenTextures(1, &skyboxTex);
	glBindTexture(GL_TEXTURE_2D, skyboxTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* skybox = SDL_LoadBMP("skybox.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, skybox->w, skybox->h, 0, GL_BGR, GL_UNSIGNED_BYTE, skybox->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(skybox);
	glBindTexture(GL_TEXTURE_2D, 0);

	//left paddle texture
	glGenTextures(1, &LeftPaddleTexture);
	glBindTexture(GL_TEXTURE_2D, LeftPaddleTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* lpImage = SDL_LoadBMP("Lpaddle.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lpImage->w, lpImage->h, 0, GL_BGR, GL_UNSIGNED_BYTE, lpImage->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(lpImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	// bounds texture
	glGenTextures(1, &boundsTexture);
	glBindTexture(GL_TEXTURE_2D, boundsTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* boundsImage = SDL_LoadBMP("bounds.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, boundsImage->w, boundsImage->h, 0, GL_BGR, GL_UNSIGNED_BYTE, boundsImage->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(boundsImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &ballTexture);
	glBindTexture(GL_TEXTURE_2D, ballTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* ballImage = SDL_LoadBMP("ball.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ballImage->w, ballImage->h, 0, GL_BGR, GL_UNSIGNED_BYTE, ballImage->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(ballImage);
	glBindTexture(GL_TEXTURE_2D, 0);


	//right paddle texture
	glGenTextures(1, &RightPaddleTexture);
	glBindTexture(GL_TEXTURE_2D, RightPaddleTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* rpImage = SDL_LoadBMP("Rpaddle.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rpImage->w, rpImage->h, 0, GL_BGR, GL_UNSIGNED_BYTE, rpImage->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(rpImage);
	glBindTexture(GL_TEXTURE_2D, 0);
	//background textures
	// right score 0
	glGenTextures(1, &Rscore0Texture);
	glBindTexture(GL_TEXTURE_2D, Rscore0Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* Rscore0Image = SDL_LoadBMP("rs0.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Rscore0Image->w, Rscore0Image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, Rscore0Image->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(Rscore0Image);
	glBindTexture(GL_TEXTURE_2D, 0);
	// right score 1
	glGenTextures(1, &Rscore1Texture);
	glBindTexture(GL_TEXTURE_2D, Rscore1Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* Rscore1Image = SDL_LoadBMP("rs1.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Rscore1Image->w, Rscore1Image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, Rscore1Image->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(Rscore1Image);
	glBindTexture(GL_TEXTURE_2D, 0);
	// right score 2
	glGenTextures(1, &Rscore2Texture);
	glBindTexture(GL_TEXTURE_2D, Rscore2Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* Rscore2Image = SDL_LoadBMP("rs2.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Rscore2Image->w, Rscore2Image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, Rscore2Image->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(Rscore2Image);
	glBindTexture(GL_TEXTURE_2D, 0);
	// right loser texture
	glGenTextures(1, &RloserTexture);
	glBindTexture(GL_TEXTURE_2D, RloserTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* RloserImage = SDL_LoadBMP("Rloser.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RloserImage->w, RloserImage->h, 0, GL_BGR, GL_UNSIGNED_BYTE, RloserImage->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(RloserImage);
	glBindTexture(GL_TEXTURE_2D, 0);
	// right winner texture
	glGenTextures(1, &RwinnerTexture);
	glBindTexture(GL_TEXTURE_2D, RwinnerTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* RwinnerImage = SDL_LoadBMP("Rwinner.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RwinnerImage->w, RwinnerImage->h, 0, GL_BGR, GL_UNSIGNED_BYTE, RwinnerImage->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(RwinnerImage);
	glBindTexture(GL_TEXTURE_2D, 0);
	// left score 0 texture
	glGenTextures(1, &Lscore0Texture);
	glBindTexture(GL_TEXTURE_2D, Lscore0Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* Lscore0Image = SDL_LoadBMP("ls0.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Lscore0Image->w, Lscore0Image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, Lscore0Image->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(Lscore0Image);
	glBindTexture(GL_TEXTURE_2D, 0);
	// left score 1 texture
	glGenTextures(1, &Lscore1Texture);
	glBindTexture(GL_TEXTURE_2D, Lscore1Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* Lscore1Image = SDL_LoadBMP("ls1.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Lscore1Image->w, Lscore1Image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, Lscore1Image->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(Lscore1Image);
	glBindTexture(GL_TEXTURE_2D, 0);
	//left score 2 texture
	glGenTextures(1, &Lscore2Texture);
	glBindTexture(GL_TEXTURE_2D, Lscore2Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* Lscore2Image = SDL_LoadBMP("ls2.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Lscore2Image->w, Lscore2Image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, Lscore2Image->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(Lscore2Image);
	glBindTexture(GL_TEXTURE_2D, 0);
	// left loser texture
	glGenTextures(1, &LloserTexture);
	glBindTexture(GL_TEXTURE_2D, LloserTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* LloserImage = SDL_LoadBMP("Lloser.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, LloserImage->w, LloserImage->h, 0, GL_BGR, GL_UNSIGNED_BYTE, LloserImage->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(LloserImage);
	glBindTexture(GL_TEXTURE_2D, 0);
	// left winner texture
	glGenTextures(1, &LwinnerTexture);
	glBindTexture(GL_TEXTURE_2D, LwinnerTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_Surface* LwinnerImage = SDL_LoadBMP("Lwinner.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, LwinnerImage->w, LwinnerImage->h, 0, GL_BGR, GL_UNSIGNED_BYTE, LwinnerImage->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(LwinnerImage);
	glBindTexture(GL_TEXTURE_2D, 0);


	//cleanup
	glDisableVertexAttribArray(positionLocation); //disable vertex attribute at index positionLocation
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind array buffer

}
// end::initializeVertexArrayObject[]

// tag::initializeVertexBuffer[]
void initializeVertexBuffer()
{
	//Left Paddle
	glGenBuffers(1, &LeftPaddleVertexDataBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, LeftPaddleVertexDataBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LeftvertexData), LeftvertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "vertexDataBufferObject created OK! GLUint is: " << LeftPaddleVertexDataBufferObject << std::endl;

	//Right Paddle
	glGenBuffers(1, &RightPaddleVertexDataBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, RightPaddleVertexDataBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RightvertexData), RightvertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "vertexDataBufferObject created OK! GLUint is: " << RightPaddleVertexDataBufferObject << std::endl;

	// Bounds
	glGenBuffers(1, &boundsVertexDataBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, boundsVertexDataBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boundsVertexData), boundsVertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "vertexDataBufferObject created OK! GLUint is: " << boundsVertexDataBufferObject << std::endl;

	// cube
	glGenBuffers(1, &cubeVertexDataBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexDataBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexData), cubeVertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "vertexDataBufferObject created OK! GLUint is: " << boundsVertexDataBufferObject << std::endl;

	// ui
	glGenBuffers(1, &rightUIVertexDataBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, rightUIVertexDataBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightUIvertexData), rightUIvertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "vertexDataBufferObject created OK! GLUint is: " << boundsVertexDataBufferObject << std::endl;

	glGenBuffers(1, &leftUIVertexDataBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, leftUIVertexDataBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftUIvertexData), leftUIvertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "vertexDataBufferObject created OK! GLUint is: " << boundsVertexDataBufferObject << std::endl;

	//skybox
	glGenBuffers(1, &skyboxVertexDataBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVertexDataBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertexData), skyboxVertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "vertexDataBufferObject created OK! GLUint is: " << skyboxVertexDataBufferObject << std::endl;

	glGenBuffers(1, &ColorDataBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, ColorDataBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColorData), cubeColorData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &TextureDataBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, TextureDataBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTextureData), cubeTextureData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	initializeVertexArrayObject();
}
// end::initializeVertexBuffer[]

// tag::loadAssets[]
void loadAssets()
{
	initializeProgram(); //create GLSL Shaders, link into a GLSL program, and get IDs of attributes and variables

	initializeVertexBuffer(); //load data into a vertex buffer

	cout << "Loaded Assets OK!\n";
}
// end::loadAssets[]

// tag::handleInput[]
void handleInput()
{
	//Event-based input handling
	//The underlying OS is event-based, so **each** key-up or key-down (for example)
	//generates an event.
	//  - https://wiki.libsdl.org/SDL_PollEvent
	//In some scenarios we want to catch **ALL** the events, not just to present state
	//  - for instance, if taking keyboard input the user might key-down two keys during a frame
	//    - we want to catch based, and know the order
	//  - or the user might key-down and key-up the same within a frame, and we still want something to happen (e.g. jump)
	//  - the alternative is to Poll the current state with SDL_GetKeyboardState

	SDL_Event event; //somewhere to store an event
	//NOTE: there may be multiple events per frame
	while (SDL_PollEvent(&event)) //loop until SDL_PollEvent returns 0 (meaning no more events)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			done = true; //set donecreate remote branch flag if SDL wants to quit (i.e. if the OS has triggered a close event,
							//  - such as window close, or SIGINT
			break;

			//keydown handling - we should to the opposite on key-up for direction controls (generally)
		case SDL_KEYDOWN:
			event.key.repeat = true;
			if (event.key.repeat)
				switch (event.key.keysym.sym)
				{
					//hit escape to exit
				case SDLK_ESCAPE: done = true;
					break;
				case SDLK_SPACE: go = true;		// make game go
												// statement resets game
												//positions reset as well as ball velocity and scores
					if (gameOver == true) {
						go = false;
						gameOver = false;
						ballPos[0] = 0.0f;
						ballPos[1] = 0.0f;
						ballVel[0] = 0.02f;
						ballVel[1] = 0.01f;
						RPscore = 0;
						LPscore = 0;
					}
					break;
				case SDLK_w: rpUp = true;
					break;
				case SDLK_s: rpDown = true;
					break;
				case SDLK_o: lpUp = true;
					break;
				case SDLK_l: lpDown = true;
					break;
				case SDLK_1: cameraStyle = 0;
					break;
				case SDLK_2: cameraStyle = 1;
					break;
				case SDLK_3: cameraStyle = 2;
					break;
				case SDLK_4: cameraStyle = 3;
					break;
				case SDLK_5: cameraStyle = 4;
					break;
				case SDLK_UP: cameraForward = true;
					break;
				case SDLK_DOWN: cameraBackward = true;
					break;
				case SDLK_LEFT: cameraLeft = true;
					break;
				case SDLK_RIGHT: cameraRight = true;
					break;
				}
			break;
		case SDL_KEYUP:
			event.key.repeat = true;
			if (event.key.repeat)
				switch (event.key.keysym.sym)
				{
				case SDLK_w: rpUp = false;
					break;
				case SDLK_s: rpDown = false;
					break;
				case SDLK_o: lpUp = false;
					break;
				case SDLK_l: lpDown = false;
					break;
				case SDLK_UP: cameraForward = false;
					break;
				case SDLK_DOWN: cameraBackward = false;
					break;
				case SDLK_LEFT: cameraLeft = false;
					break;
				case SDLK_RIGHT: cameraRight = false;
					break;
				}
			break;			
		}
	}
}
// end::handleInput[]



// tag::updateSimulation[]
void updateSimulation(double simLength = 0.02) //update simulation with an amount of time to simulate for (in seconds)
{
	//WARNING - we should calculate an appropriate amount of time to simulate - not always use a constant amount of time
			// see, for example, http://headerphile.blogspot.co.uk/2014/07/part-9-no-more-delays.html
	if (go == true) {
		ballPos += ballVel;
	}

	float rotate = (float)simLength * rotateSpeed; //simlength is a double for precision, but rotateSpeedVector in a vector of float, alternatively use glm::dvec3

												   //modify the rotateMatrix with the rotate, as a rotate, around the z-axis
	const glm::vec3 unitX = glm::vec3(1, 0, 0);
	const glm::vec3 unitY = glm::vec3(0, 1, 0);
	const glm::vec3 unitZ = glm::vec3(0, 0, 1);
	const glm::vec3 unit45 = glm::normalize(glm::vec3(0, 1, 1));
	rotateMatrix = glm::rotate(rotateMatrix, rotate, unit45);
	camZ += 0.001f * radius;
	camX += 0.001f * radius;

	skyBoxUp = glm::vec3(cameraUp.x / 10, cameraUp.y / 10, cameraUp.z / 10);
	skyBoxRotatematrix = glm::rotate(skyBoxRotatematrix, 0.0f, cameraUp);
	padLmatrix = glm::translate(glm::mat4(1.0f), padLpos);
	padRmatrix = glm::translate(glm::mat4(1.0f), padRpos);
	ballMatrix = glm::translate(glm::mat4(1.0f), ballPos);

	lightPosition = glm::vec3(lightPosition.x, lightPosition.y, lightPosition.z + lightMove);
	
	if (lightPosition.z >= 1.0)
	{
		lightMove = lightMove * -1;
	}
	if (lightPosition.z <= -1.0)
	{
		lightMove = lightMove * -1;
	}
	lightMatrix = glm::translate(glm::mat4(1.0f), lightPosition);

	if (cameraForward == true) {
		cameraPosition -= cameraSpeed * cameraFront;
	}
	if (cameraBackward == true) {
		cameraPosition += cameraSpeed * cameraFront;
	}
	if (cameraLeft == true) {
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (cameraRight == true) {
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (rpDown == true)
	{
		padRpos[1] -= 0.02f;
	}
	if (rpUp == true)
	{
		padRpos[1] += 0.02f;
	}
	if (lpDown == true)
	{
		padLpos[1] -= 0.02f;
	}
	if (lpUp == true)
	{
		padLpos[1] += 0.02f;
	}
	if (padRpos[1] >= 0.80) {
		padRpos[1] = 0.80;
	}
	if (padRpos[1] <= -0.80) {
		padRpos[1] = -0.80;
	}
	if (padLpos[1] >= 0.80) {
		padLpos[1] = 0.80;
	}
	if (padLpos[1] <= -0.80) {
		padLpos[1] = -0.80;
	}
	if (ballPos[1] >= 0.90) {
		ballVel[1] = ballVel[1] * -1;
	}
	if (ballPos[1] <= -0.90) {
		ballVel[1] = ballVel[1] * -1;
	}
	if (ballPos[0] >= 0.90) {
		ballPos[0] = 0.0f;
		ballPos[1] = 0.0f;
		LPscore++;
		go = false;
	}
	if (ballPos[0] <= -0.90) {
		ballPos[0] = 0.0f;
		ballPos[1] = 0.0f;
		LPscore++;
		go = false;
	}
	if (ballPos[0] >= 0.75f && ballPos[1] <= padRpos[1] + 0.15f && ballPos[1] >= padRpos[1] - 0.15f) {
		ballVel[0] = -ballVel[0];
	}
	if (ballPos[0] <= -0.75f && ballPos[1] <= padLpos[1] + 0.15f && ballPos[1] >= padLpos[1] - 0.15f) {
		ballVel[0] = -ballVel[0];
	}



	skyBoxPosition = cameraPosition;
	skyBoxmatrix = glm::translate(glm::mat4(1.0f), skyBoxPosition);
}
// end::updateSimulation[]

// tag::preRender[]
void preRender()
{
	glViewport(0, 0, 600, 600); //set viewpoint
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f); //set clear colour
	glClear(GL_COLOR_BUFFER_BIT); //clear the window (technical the scissor box bounds)
}
// end::preRender[]

// tag::render[]
void render()
{
	glUseProgram(theProgram);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform3f(lightColorLocation, lightColor[0], lightColor[1], lightColor[2]);
	glUniform3f(lightPositionLocation, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(cameraPositionLocation, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	/////////

	if (RPscore == 0) {
		glBindTexture(GL_TEXTURE_2D, Rscore0Texture);
	}
	if (RPscore == 1) {
		glBindTexture(GL_TEXTURE_2D, Rscore1Texture);
	}
	if (RPscore == 2) {
		glBindTexture(GL_TEXTURE_2D, Rscore2Texture);
	}
	if (RPscore == 3) {
		glBindTexture(GL_TEXTURE_2D, RwinnerTexture);
	}
	if (LPscore == 3) {
		glBindTexture(GL_TEXTURE_2D, RloserTexture);
		gameOver = true;
	}
	glBindVertexArray(rightUIVertexArrayObject);
	glUniformMatrix4fv(rotateMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(projectionMatrixLocation, 1, false, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(glm::mat4(1.0f)));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


	if (LPscore == 0) {
		glBindTexture(GL_TEXTURE_2D, Lscore0Texture);
	}
	if (LPscore == 1) {
		glBindTexture(GL_TEXTURE_2D, Lscore1Texture);
	}
	if (LPscore == 2) {
		glBindTexture(GL_TEXTURE_2D, Lscore2Texture);
	}
	if (LPscore == 3) {
		glBindTexture(GL_TEXTURE_2D, LwinnerTexture);
	}
	if (RPscore == 3) {
		glBindTexture(GL_TEXTURE_2D, LloserTexture);
		gameOver = true;
	}
	glBindVertexArray(leftUIVertexArrayObject);
	glUniformMatrix4fv(rotateMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(projectionMatrixLocation, 1, false, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(glm::mat4(1.0f)));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	/////////
	glm::mat4 projection;
	projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionMatrixLocation, 1, false, glm::value_ptr(projection));

	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f) + cameraPosition, glm::vec3(ballPos.x, ballPos.y, 0.0f), cameraUp);
	view1 = glm::lookAt(glm::vec3(2.0f, 0.0f, 1.0f) + cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), cameraUp);
	view2 = glm::lookAt(glm::vec3(-2.0f, 0.0f, 1.0f) + cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), cameraUp);
	view3 = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f) + cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), cameraUp);
	view4 = glm::lookAt(cameraPosition, cameraPosition - cameraFront, cameraUp);
	if (cameraStyle == 0) {
		glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(view));
		cameraUp = glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	if (cameraStyle == 1) {
		glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(view1));
		cameraUp = glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	if (cameraStyle == 2) {
		glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(view2));
		cameraUp = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	if (cameraStyle == 3) {
		glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(view3));
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	if (cameraStyle == 4) {
		glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(view4));
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	
	glBindTexture(GL_TEXTURE_2D, boundsTexture);
	glBindVertexArray(boundsVertexArrayObject);
	glUniformMatrix4fv(rotateMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(glm::mat4(1.0f)));
	glDrawArrays(GL_TRIANGLES, 0, 144);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	glBindVertexArray(lightVertexArrayObject);
	glUniformMatrix4fv(rotateMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(lightMatrix));
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, ballTexture);
	glBindVertexArray(cubeVertexArrayObject);
	glUniformMatrix4fv(rotateMatrixLocation, 1, GL_FALSE, glm::value_ptr(rotateMatrix));
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(ballMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, LeftPaddleTexture);
	glBindVertexArray(LeftPaddleVertexArrayObject);
	glUniformMatrix4fv(rotateMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(padLmatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, RightPaddleTexture);
	glBindVertexArray(RightPaddleVertexArrayObject);
	glUniformMatrix4fv(rotateMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(padRmatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, skyboxTex);
	glBindVertexArray(skyboxVertexArrayObject);
	glUniformMatrix4fv(rotateMatrixLocation, 1, GL_FALSE, glm::value_ptr(skyBoxRotatematrix));
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(skyBoxmatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	

	glUseProgram(0); //clean up
}
// end::render[]

// tag::postRender[]
void postRender()
{
	SDL_GL_SwapWindow(win);; //present the frame buffer to the display (swapBuffers)
	frameLine += "Frame: " + std::to_string(frameCount++);
	cout << "\r" << frameLine << std::flush;
	frameLine = "";
}
// end::postRender[]

// tag::cleanUp[]
void cleanUp()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	cout << "Cleaning up OK!\n";
}
// end::cleanUp[]

// tag::main[]
int main( int argc, char* args[] )
{
	exeName = args[0];
	//setup
	//- do just once
	initialise();
	createWindow();

	createContext();

	initGlew();

	glViewport(0,0,600,600); //should check what the actual window res is?

	SDL_GL_SwapWindow(win); //force a swap, to make the trace clearer

	//do stuff that only needs to happen once
	//- create shaders
	//- load vertex data
	loadAssets();

	while (!done) //loop until done flag is set)
	{
		handleInput(); // this should ONLY SET VARIABLES
		updateSimulation(); // this should ONLY SET VARIABLES according to simulation

		preRender();

		render(); // this should render the world state according to VARIABLES -

		postRender();

	}

	//cleanup and exit
	cleanUp();
	SDL_Quit();

	return 0;
}
// end::main[]
