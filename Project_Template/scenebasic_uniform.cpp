#include "scenebasic_uniform.h"

#include <iostream>
#include <Windows.h>

#include<sstream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

#include "helper/texture.h"
#include "helper/scenerunner.h"
#include <time.h>





glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //defining controls for camera

SceneBasic_Uniform::SceneBasic_Uniform() : plane(10.0f, 10.0f, 1000, 1000), angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f)
{

	cube = ObjMesh::load("media/cube.obj",
		true);
	pig = ObjMesh::load("media/pig_triangulated.obj",
		true);
	wall = ObjMesh::load("media/wall.obj",
		true);
	ogre = ObjMesh::load("media/bs_ears.obj",
		true);


}






void SceneBasic_Uniform::initScene()
{
	compile();
	prog2.use();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    numSprites = 50;
    locations = new float[numSprites * 3];
    srand((unsigned int)time(0));

    for (int i = 0; i < numSprites; i++)
    {
        vec3 p(((float)rand() / RAND_MAX * 2.0f) - 1.0f,
            ((float)rand() / RAND_MAX * 2.0f) - 1.0f,
            ((float)rand() / RAND_MAX * 2.0f) - 1.0f);

        locations[i * 3] = p.x;
        locations[i * 3 + 1] = p.y;
        locations[i * 3 + 2] = p.z;
    }

    GLuint handle;
    glGenBuffers(1, &handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, numSprites * 3 * sizeof(float), locations, GL_STATIC_DRAW);

    delete[] locations;

    glGenVertexArrays(1, &Sprites);
    glBindVertexArray(Sprites);

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);


	GLuint texID5 =
		Texture::loadTexture("media/texture/flower.png");
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texID5);
   /* const char* texName = "media/texture/flower.png";
    Texture::loadTexture(texName);*/

    prog2.setUniform("SpriteTex", 0);
    prog2.setUniform("Size2", 0.1f);
	prog.use();



	view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f));
	//projection = mat4(1.0f);
	float x, z;
	for (int i = 0; i < 3; i++)
	{
		std::stringstream name;
		name << "Lights[" << i << "].Position";
		x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
		z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
		prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z +
			1.0f, 1.0f));
	}


	glEnable(GL_DEPTH_TEST);
	view = glm::lookAt(vec3(1.0f, 1.25f, 1.25f), vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f));
	//projection = mat4(1.0f);


	prog.setUniform("Lights[0].L", vec3(0.0f, 0.0f, 0.8f));
	prog.setUniform("Lights[1].L", vec3(0.0f, 0.8f, 0.0f));
	prog.setUniform("Lights[2].L", vec3(0.8f, 0.0f, 0.0f));
	prog.setUniform("Lights[0].La", vec3(0.0f, 0.0f, 0.8f));
	prog.setUniform("Lights[1].La", vec3(0.0f, 0.8f, 0.0f));
	prog.setUniform("Lights[2].La", vec3(0.8f, 0.0f, 0.0f));
	prog.setUniform("light.l", vec3(0.9, 0.9, 0.9));
	prog.setUniform("light.la", vec3(0.6, 0.4, 0.3));


	/// <summary>
	///  the below code loads both textures into the scene and combines them into one so they are textured differently
	/// </summary>
	GLuint texID =
		Texture::loadTexture("media/texture/brick1.jpg");
	GLuint texID3 =
		Texture::loadTexture("media/texture/cement.jpg");
	GLuint texID2 =
		Texture::loadTexture("media/texture/moss.png");
	GLuint texID4 =
		Texture::loadTexture("media/texture/ogre_diffuse.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texID2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texID3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texID4);







	//projection = mat4(1.0f);
	angle = glm::radians(90.0f); //set the initial angle
	//extract the cube texture
	GLuint cubeTex =
		Texture::loadHdrCubeMap("media/texture/cube/pisa-hdr/pisa");
	//activate and bindtexture
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);







}

void SceneBasic_Uniform::compile()
{
	try {



		
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");//here the two shaders are loadead in with the compile 
		//prog.compileShader("shader/basic_uniform.geom");
		prog.link();
		prog.use();

		prog2.compileShader("shader/basic_uniform_pointsprite.vert");
		prog2.compileShader("shader/basic_uniform_pointsprite.frag");
		prog2.compileShader("shader/basic_uniform_pointsprite.geom");
		prog2.link();
		prog2.use();



		prog3.compileShader("shader/basic_uniform_skybox.vert");
		prog3.compileShader("shader/basic_uniform_skybox.frag");
		prog3.link();
	}
	catch (GLSLProgramException& e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}



	
}

void SceneBasic_Uniform::update(float t)
{
	//camera controls using get key states to manipulate the camera
	float x = 0.0f;
	if (GetKeyState('A') & 0x8000) //0x8000 means it is a short press
	{
		x = 100.f;
		float deltaT = t - tPrev;
		if (tPrev == x)
			deltaT = 10.0f;
		tPrev = t;
		angle += rotSpeed * deltaT;
		if (angle > glm::two_pi<float>())
			angle -= glm::two_pi<float>();
	}

	if (GetKeyState('D') & 0x8000)
	{

		float deltaT = tPrev - t; //inverted to make it go the other way around 
		if (tPrev == x)
			deltaT = 100.0f;
		tPrev = t;
		angle += rotSpeed * deltaT;
		if (angle > glm::two_pi<float>())
			angle -= glm::two_pi<float>();
	}

	if (GetKeyState('P') & 0x8000) { //change textures on a button press
	  /*  GLuint texID =
			Texture::loadTexture("media/texture/spencer.jpg");
		GLuint texID2 =
			Texture::loadTexture("media/texture/moss.png");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texID2);*/
	}

	if (GetKeyState('L') & 0x8000) { //change textures on a button press
	   /* GLuint texID =
			Texture::loadTexture("media/texture/cement.jpg");
		GLuint texID2 =
			Texture::loadTexture("media/texture/bluewater.png");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texID2);*/

	}

	if (GetKeyState('M') & 0x8000) { //change textures on a button press
	  /*  GLuint texID =
			Texture::loadTexture("media/texture/brick1.jpg");
		GLuint texID3 =
			Texture::loadTexture("media/texture/cement.jpg");
		GLuint texID2 =
			Texture::loadTexture("media/texture/moss.png");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texID2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texID3);*/
	}


	if (GetKeyState('E') & 0x8000) {
		
		

	}




}

void SceneBasic_Uniform::render()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	prog2.use();

	float x = 2.0f; //declaring values for camera start poistion 
	float y = 1.0f;

	/*vec3 cameraPos(1.0f, 0.0f, 1.0f);
	view = glm::lookAt(cameraPos,
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f));*/


	vec3 cameraPos = vec3(10.0f * cos(angle), x, y * sin(angle));
	view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f,
		0.0f));
	//point sprite

	model = mat4(1.0f);
	//model = glm::scale(model, vec3(10.0));
	setMatrices();

	glBindVertexArray(Sprites);
	glDrawArrays(GL_POINTS, 0, numSprites);

	


	prog.use();


	prog.setUniform("Tex1", 0); //feeding Tex 1 to set to 0 for it to grab the correct texture


	prog.setUniform("light.position", vec3(0.0, 1.0, 0.0));
	prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
	prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
	prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
	prog.setUniform("Material.Shininess", 180.0f);
	model = mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));


	setMatrices();
	pig->render();
	model = mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, vec3(3.0f, 0, 0)); //moving the cube out of the way for the pig to not be inside it 


	setMatrices();
	cube->render();//cube is differently name in the .h file so they are not overwiring eachother
	model = mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, vec3(-3.0f, 0, 0)); //moving the wall
	setMatrices();
	wall->render();//wall is differently name in the .h file so they are not overwiring eachother



	prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
	prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
	prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
	prog.setUniform("Material.Shininess", 180.0f);
	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
	setMatrices();
	prog.setUniform("Tex1", 2);
	plane.render();

	model = mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, vec3(1.0f, 2, 0));
	setMatrices();
	prog.setUniform("Tex1", 3);
	ogre->render();

	

	prog3.use();
	model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	model = mat4(1.0f);
	prog.setUniform("SkyBoxTex", 4);
	setMatrices();
	sky.render();



	glFinish();


}




void SceneBasic_Uniform::setMatrices()
{


	


	mat4 mv = view * model; //we create a model view matrix

	prog2.setUniform("ModelViewMatrix", mv); //set the uniform for the model view matrix

	prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]))); //we set the uniform for normal matrix

	prog2.setUniform("ProjectionMatrix", projection);

	prog.setUniform("MVP", projection * mv); //we set the model view matrix by multiplying the mv with the projection matrix
}

void SceneBasic_Uniform::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}
