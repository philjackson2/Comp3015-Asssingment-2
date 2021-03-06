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
#include "helper/glutils.h"


float flowerSize = 0.1f; // float to enable the changing of the size of the flower.

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //defining controls for camera

SceneBasic_Uniform::SceneBasic_Uniform() : plane(10.0f, 10.0f, 1000, 1000), angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f), Time(0), particleLifetime(100), nParticles(5000), emitterPos(-0.30, 2., -0.30), emitterDir(2, -2.3, 2)
{

	flowerPot = ObjMesh::load("media/flower pot.obj",
		true);

	wateringCan = ObjMesh::load("media/wateringcan.obj",
		true);
	


}


void SceneBasic_Uniform::initScene()
{

	compile();

	initBuffers();
		glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		GLuint texID =
		Texture::loadTexture("media/texture/metal.jpg");
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

	glActiveTexture(GL_TEXTURE5);
	Texture::loadTexture("media/texture/bluewater.png");
			
	angle = glm::radians(90.0f); //set the initial angle
	//extract the cube texture
	GLuint cubeTex =
		Texture::loadHdrCubeMap("media/texture/cube/pisa-hdr/pisa");
	//activate and bindtexture
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

	
#pragma region PointSprite

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		glEnable(GL_DEPTH_TEST);


		numSprites = 30;
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
		glBufferData(GL_ARRAY_BUFFER, numSprites * 8 * sizeof(float), locations, GL_STATIC_DRAW);

		delete[] locations;

		glGenVertexArrays(1, &Sprites);
		glBindVertexArray(Sprites);

		glBindBuffer(GL_ARRAY_BUFFER, handle);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE6);
		const char* texName = "media/texture/flower.png";
		Texture::loadTexture(texName);


		prog2.use();

		prog2.setUniform("Size2", flowerSize); // change the size of the point sprites

	
#pragma endregion



#pragma region Lights

	prog.use();

	view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f));
	
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
	
	prog.setUniform("Lights[0].L", vec3(0.0f, 0.0f, 0.8f));
	prog.setUniform("Lights[1].L", vec3(0.0f, 0.8f, 0.0f));
	prog.setUniform("Lights[2].L", vec3(0.8f, 0.0f, 0.0f));
	prog.setUniform("Lights[0].La", vec3(0.0f, 0.0f, 0.8f));
	prog.setUniform("Lights[1].La", vec3(0.0f, 0.8f, 0.0f));
	prog.setUniform("Lights[2].La", vec3(0.8f, 0.0f, 0.0f));
	prog.setUniform("light.l", vec3(0.9, 0.9, 0.9));
	prog.setUniform("light.la", vec3(0.6, 0.4, 0.3));

#pragma endregion
			
}

void SceneBasic_Uniform::compile()
{
	try {

		//prog is used to load in the different shaders, there are different progs for each shader of every and they are called upon as they are needed

		
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");//here the two shaders are loadead in with the compile 

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
			

		prog5.compileShader("shader/basic_uniform_particle.vert");
		prog5.compileShader("shader/basic_uniform_particle.frag");
		prog5.link();
		flatprog.compileShader("shader/flat_frag.glsl");
		flatprog.compileShader("shader/flat_vert.glsl");
		flatprog.link();
		
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

	if (GetKeyState('P')) 
	{ 
		//maniplate the size of the flower!
		flowerSize = flowerSize + 0.0001f;

		prog2.use();
		prog2.setUniform("Size2", flowerSize);

	}
	Time = t;


	}

void SceneBasic_Uniform::render()
{

	float x = 2.0f; //declaring values for camera start poistion 
	float y = 1.0f;


	vec3 cameraPos = vec3(10.0f * cos(angle), x, y * sin(angle));
	view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f,
		0.0f));

#pragma region Point Sprite

	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	prog2.use();
	
	model = mat4(1.0f);
	
	setMatrices(prog2);
	prog2.setUniform("SpriteTex", 6);
	glBindVertexArray(Sprites);
	glDrawArrays(GL_POINTS, 0, numSprites);

	

#pragma endregion


	prog.use();


	prog.setUniform("Tex1", 0); 


	prog.setUniform("light.position", vec3(0.0, 1.0, 0.0));
	prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
	prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
	prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
	prog.setUniform("Material.Shininess", 180.0f);
	
	model = mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, vec3(3.0f, 0, 0)); 

	setMatrices(prog);
	flowerPot->render();


	model = mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), vec3(5.0f, 1.0f, 0.0f));
	model = glm::translate(model, vec3(0.0f, -1.4, -3.5)); 
	setMatrices(prog);
	wateringCan->render();



	prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
	prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
	prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
	prog.setUniform("Material.Shininess", 180.0f);
	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
	setMatrices(prog);
	prog.setUniform("Tex1", 2);
	plane.render();

	

	

	prog3.use();
	model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	model = mat4(1.0f);
	prog.setUniform("SkyBoxTex", 4);
	setMatrices(prog);
	sky.render();





#pragma region Particle
	if (GetKeyState('P') & 0x8000) {

		model = mat4(1.0f);
		flatprog.use();
		flatprog.setUniform("Colour", glm::vec4(1.4f, 0.4f, 0.4f, 3.0f));


		glDepthMask(GL_FALSE);
		prog5.use();
		prog5.setUniform("ParticleTex", 5);
		prog5.setUniform("ParticleLifetime", particleLifetime);
		prog5.setUniform("ParticleSize", 0.05f);
		prog5.setUniform("Gravity", vec3(0.0f, -0.2f, 0.0f));
		prog5.setUniform("EmitterPos", emitterPos);
		setMatrices(prog5);

		prog5.setUniform("Time", Time);
		glBindVertexArray(particles);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);
	}
#pragma endregion

	glFinish();


}




void SceneBasic_Uniform::setMatrices(GLSLProgram& p)
{
			
	mat4 mv = view * model; //we create a model view matrix

	p.setUniform("ModelViewMatrix", mv); //set the uniform for the model view matrix

	p.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]))); //we set the uniform for normal matrix

	p.setUniform("ProjectionMatrix", projection);

	p.setUniform("MVP", projection * mv); //we set the model view matrix by multiplying the mv with the projection matrix


	p.setUniform("MV", mv);

	
}

void SceneBasic_Uniform::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}


float SceneBasic_Uniform::randFloat()
{
	return Rand.nextFloat();
}

void SceneBasic_Uniform::initBuffers()
{
	// generate the buffers
	glGenBuffers(1, &initVel); // initial velocity buffer
	glGenBuffers(1, &startTime); // start time buffer

	
	int size = nParticles * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size * 3, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
	
	// give random velocities to the buffer
	glm::mat3 emitterBasis = ParticleUtils::makeArbitraryBasis(emitterDir);
	vec3 v(0.0f);
	float velocity, theta, phi;
	std::vector<GLfloat> data(nParticles * 3);

	for (uint32_t i = 0; i < nParticles; i++)
	{
		
		theta = glm::mix(0.0f, glm::pi<float>() / 20.0f, randFloat());
		phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		
		velocity = glm::mix(1.25f, 1.5f, randFloat());
		v = glm::normalize(emitterBasis * v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}

	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3, data.data()); 

	
	float rate = particleLifetime / nParticles;
	for (int i = 0; i < nParticles; i++)
	{
		data[i] = rate * i;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data.data()); 

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &particles);
	glBindVertexArray(particles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(1);

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);

	glBindVertexArray(0);
}