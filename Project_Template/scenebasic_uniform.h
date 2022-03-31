#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"


#include <glm/glm.hpp>
#include "helper/plane.h"
#include "helper/objmesh.h"

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;


    float angle, tPrev, rotSpeed;
   

    Plane plane; 
    std::unique_ptr<ObjMesh> pig; //pig mesh
    std::unique_ptr<ObjMesh> cube; //for the cube
    std::unique_ptr<ObjMesh> wall;//wall
    void setMatrices();

    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
