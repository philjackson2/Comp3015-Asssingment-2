#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include "helper/skybox.h"

#include <glm/glm.hpp>
#include "helper/plane.h"
#include "helper/objmesh.h"

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;
  
    SkyBox sky;

    GLuint fsQuad, fboHandle, renderTex;

    float angle, tPrev, rotSpeed;
   

    Plane plane; 
    std::unique_ptr<ObjMesh> pig; //pig mesh
    std::unique_ptr<ObjMesh> cube; //for the cube
    std::unique_ptr<ObjMesh> wall;//wall
    std::unique_ptr<ObjMesh> ogre;
    void setMatrices();

    void compile();


    void setupFBO();

    void pass1();
    void pass2();

    void renderEdgeDetect();

    void edgeDetectShaders();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
