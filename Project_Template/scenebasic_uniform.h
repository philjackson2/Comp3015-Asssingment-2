#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include "helper/skybox.h"

#include <glm/glm.hpp>
#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/random.h"
#include "helper/particleutils.h"
#include <GLFW/glfw3.h>

class SceneBasic_Uniform : public Scene
{
private:
	GLSLProgram prog;
	GLSLProgram prog2;
	GLSLProgram prog3;
	GLSLProgram prog4;
	GLSLProgram prog5, flatprog;

	float AnimationAngle;
	float AnimationTime; 


	GLuint Sprites;
	int numSprites;
	float* locations; 






	Random Rand;

	GLuint initVel, startTime, particles, nParticles;

	float Time, particleLifetime;
	

	glm::vec3 emitterPos, emitterDir;



	SkyBox sky;



	float angle, tPrev, rotSpeed;


	Plane plane;
	std::unique_ptr<ObjMesh> pig; //pig mesh
	std::unique_ptr<ObjMesh> cube; //for the cube
	std::unique_ptr<ObjMesh> wateringCan;//wateringcan
	std::unique_ptr<ObjMesh> ogre;

	void setMatrices(GLSLProgram&);

	void compile();

public:
	SceneBasic_Uniform();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
	void render2();
	void initBuffers();
	float randFloat();
	
};

#endif // SCENEBASIC_UNIFORM_H
