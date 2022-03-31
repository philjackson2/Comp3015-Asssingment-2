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
    
    
}






void SceneBasic_Uniform::initScene()
{
    compile();
	glEnable(GL_DEPTH_TEST);


    view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);
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
    projection = mat4(1.0f);


    prog.setUniform("Lights[0].L", vec3(0.0f, 0.0f, 0.8f));
    prog.setUniform("Lights[1].L", vec3(0.0f, 0.8f, 0.0f));
    prog.setUniform("Lights[2].L", vec3(0.8f, 0.0f, 0.0f));
    prog.setUniform("Lights[0].La", vec3(0.0f, 0.0f, 0.8f));
    prog.setUniform("Lights[1].La", vec3(0.0f, 0.8f, 0.0f));
    prog.setUniform("Lights[2].La", vec3(0.8f, 0.0f, 0.0f));
    prog.setUniform("light.l", vec3(0.9, 0.9, 0.9));
    prog.setUniform("light.la", vec3(0.6, 0.4, 0.3));

    GLuint texID =
        Texture::loadTexture("media/texture/brick1.jpg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);

   
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");//here the two shaders are loadead in with the compile 
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
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
        GLuint texID =
            Texture::loadTexture("media/texture/spencer.jpg");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);
    }

    if (GetKeyState('L') & 0x8000) { //change textures on a button press
        GLuint texID =
            Texture::loadTexture("media/texture/cement.jpg");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);
    }

    if (GetKeyState('M') & 0x8000) { //change textures on a button press
        GLuint texID =
            Texture::loadTexture("media/texture/brick1.jpg");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);
    }
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
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
    wall ->render();//wall is differently name in the .h file so they are not overwiring eachother

    prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 180.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    setMatrices();
    plane.render();

    float x = 2.0f; //declaring values for camera start poistion 
    float y = 1.0f;

    vec3 cameraPos = vec3(10.0f * cos(angle),x, y * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f,
        0.0f));

}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model; //we create a model view matrix
    
    prog.setUniform("ModelViewMatrix", mv); //set the uniform for the model view matrix
    
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]))); //we set the uniform for normal matrix
    
    prog.setUniform("MVP", projection * mv); //we set the model view matrix by multiplying the mv with the projection matrix
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}
