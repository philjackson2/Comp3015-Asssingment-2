# Version Information 
Written in Visual Studio 2019 Version 16.11.7 
Targeted for Windows 10 
Configuration type  Application (.exe)
Windows SDK 10.0


# How it works and what makes it unique
This is called 'Italian Gardener'. You are a gardener in the historical Italian city of Pisa! Take your watering can and press 'P' to pour water on your growing garden. Observe around it using the 'A' and 'D' keys and watch as your garden grows. This acts as a gardening simulator. 

This project is based on the previous project, it uses the same shaders to render in 3D objects to the scene, but added shaders for a skybox, point sprites and particle emitter. When implementing these additional features it looked like plants being watered, so rolling with this a function to have the point sprites (textured as flowers) increase in size as the user pours water onto them using the particle emitter. 


In order to run simply unzip the project and click on the .exe file and it will run 



## Methods

This project mainly compiles and runs functions within the scene_runner.cpp 

These  are:

### SceneBasic_Uniform::SceneBasic_Uniform() 
This loads in the 3d objects and particle animation into the scene for it to be called upon later 


### SceneBasic_Uniform::initScene()
This initializes textures, lights and the points sprites within the scene. 

### SceneBasic_Uniform::compile()
Compiles the shaders to be called upon within other methods 

### SceneBasic_Uniform::update(float t)
This updates the scene and handles user interaction through if (GetKeyState) for when a user presses the defined key 
### SceneBasic_Uniform::render()
Renders loaded in objects, animations and point sprites into the scene as well as defining the camera and its starting position 
### SceneBasic_Uniform::setMatrices(GLSLProgram& p)
Defines fucntions that were made within shader and are called upon within other parts of the program. IT makes use of GLSLprogram so it does not have to define these functions multiple times as they are made multiple times in each shader file 



### SceneBasic_Uniform::resize(int w, int h)
This resizes the user window
### SceneBasic_Uniform::randFloat()
defining the random float value 
### SceneBasic_Uniform::initBuffers()
This function is where the particle animation is coded for it to be rendered within the render function 

# Links
https://github.com/philjackson2/Comp3015-Asssingment-2.git
https://youtu.be/BvAoPtTke-o
