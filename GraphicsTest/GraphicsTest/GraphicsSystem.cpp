#include "GraphicsSystem.h"
#include "EditorSystem.h"
#include "GraphicsComponent.h"
#include "Object.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "InputSystem.h"
#include "Core.h"
#include "Globals.h"
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

#include "SOIL.h"

glm::mat4 setUpRotationMatrix(glm::mat4 rotationMatrix, float angle, float u, float v, float w)
{
  float L = (u*u + v * v + w * w);
  angle = angle * 3.14159 / 180.0; //converting to radian value
  float u2 = u * u;
  float v2 = v * v;
  float w2 = w * w;

  rotationMatrix[0][0] = (u2 + (v2 + w2) * cos(angle)) / L;
  rotationMatrix[1][0] = (u * v * (1 - cos(angle)) - w * sqrt(L) * sin(angle)) / L;
  rotationMatrix[2][0] = (u * w * (1 - cos(angle)) + v * sqrt(L) * sin(angle)) / L;
  rotationMatrix[3][0] = 0.0;

  rotationMatrix[0][1] = (u * v * (1 - cos(angle)) + w * sqrt(L) * sin(angle)) / L;
  rotationMatrix[1][1] = (v2 + (u2 + w2) * cos(angle)) / L;
  rotationMatrix[2][1] = (v * w * (1 - cos(angle)) - u * sqrt(L) * sin(angle)) / L;
  rotationMatrix[3][1] = 0.0;

  rotationMatrix[0][2] = (u * w * (1 - cos(angle)) - v * sqrt(L) * sin(angle)) / L;
  rotationMatrix[1][2] = (v * w * (1 - cos(angle)) + u * sqrt(L) * sin(angle)) / L;
  rotationMatrix[2][2] = (w2 + (u2 + v2) * cos(angle)) / L;
  rotationMatrix[3][2] = 0.0;

  rotationMatrix[0][3] = 0.0;
  rotationMatrix[1][3] = 0.0;
  rotationMatrix[2][3] = 0.0;
  rotationMatrix[3][3] = 1.0;
  return rotationMatrix;
}


GraphicsSystem::GraphicsSystem()
{
  mName_ = "GraphicsSystem";
}


bool GraphicsSystem::Initialize()
{
  // Initialise GLFW
  if (!glfwInit())
  {
    return false;
  }
  

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); //We don't want the old OpenGL 

  // Open a window and create its OpenGL context
  width = 100, height = 100;
  mWindow = glfwCreateWindow(width, height, "Tutorial 03 - Matrices", NULL/*glfwGetPrimaryMonitor()*/, NULL);
  if (mWindow == NULL){
    return false;
  }

  // Setup ImGui binding
  ImGui_ImplGlfwGL3_Init(mWindow, true);


  glfwMakeContextCurrent(mWindow);
  glfwSetKeyCallback(mWindow, inputKeyCallback);
  glfwSetCursorPosCallback(mWindow, inputMouseCallback);
  glfwSetMouseButtonCallback(mWindow, inputButtonCallback);
  //glfwGetFramebufferSize(mWindow, &width, &height);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    return false;
  }

  GLfloat tVertexArray[] = {
    -0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
  };

  GLfloat tUVArray[] = {
    0.f, 1.f,
    0.f, 0.f,
    1.f, 0.f,
    0.f, 1.f,
    1.f, 0.f,
    1.f, 1.f,
  };

  memcpy(mVertexArray, tVertexArray, sizeof(GLfloat)* 18);
  memcpy(mUVArray, tUVArray, sizeof(GLfloat)* 12);

  glGenVertexArrays(1, &mVertexArrayID);
  glBindVertexArray(mVertexArrayID);
  glGenBuffers(1, &mVertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexArray), &mVertexArray, GL_STATIC_DRAW);


  glGenBuffers(1, &mUVbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, mUVbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(mUVArray), mUVArray, GL_STATIC_DRAW);

  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  //glEnable(GL_SCISSOR_TEST);
  glActiveTexture(GL_TEXTURE0);


  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, GL_TRUE);
  
  

  programID = LoadShaders("SimpleTransform.vertexshader", "SingleColor.fragmentshader");


  return true;
}

GLuint GraphicsSystem::LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
  if (VertexShaderStream.is_open()){
    std::string Line = "";
    while (getline(VertexShaderStream, Line))
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }
  else{
    printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
    getchar();
    return 0;
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
  if (FragmentShaderStream.is_open()){
    std::string Line = "";
    while (getline(FragmentShaderStream, Line))
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;


  // Compile Vertex Shader
  printf("Compiling shader : %s\n", vertex_file_path);
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0){
    std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    printf("%s\n", &VertexShaderErrorMessage[0]);
  }



  // Compile Fragment Shader
  printf("Compiling shader : %s\n", fragment_file_path);
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0){
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    printf("%s\n", &FragmentShaderErrorMessage[0]);
  }



  // Link the program
  printf("Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0){
    std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    printf("%s\n", &ProgramErrorMessage[0]);
  }


  glDetachShader(ProgramID, VertexShaderID);
  glDetachShader(ProgramID, FragmentShaderID);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;

}

void GraphicsSystem::GatherFrameData(GraphicsComponent * iter)
{

}

void GraphicsSystem::Update(double dt)
{
  GLfloat color[3] = { 1, 1, 0 };

  float fov = 45.f;

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Use our shader
  glUseProgram(programID);
  glActiveTexture(GL_TEXTURE0);


  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  //Projection = glm::perspective(fov, width / height, 0.1f, 100.0f);
  Projection = glm::ortho(-5.f, 5.f, -5.f, 5.f, 0.1f, 100.0f);
  //glm::mat4 Projection = glm::ortho(-4, 4, 4, -4);
  // Or, for an ortho camera :
  //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

  // Camera matrix
  View = glm::lookAt(
    glm::vec3(0, 0, 10), // Camera is at (0,0,10), in World Space
    glm::vec3(0, 0, 0), // and looks at the origin
    glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

  GLuint MatrixID = glGetUniformLocation(programID, "View");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &View[0][0]);
  MatrixID = glGetUniformLocation(programID, "Projection");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Projection[0][0]);
  GLuint ColorID = glGetUniformLocation(programID, "Color");
  // Get a handle for our "myTextureSampler" uniform
  mTextureID = glGetUniformLocation(programID, "myTextureSampler");
  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexbuffer);
  glVertexAttribPointer(
    0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );
  // 2nd attribute buffer : UVs
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, mUVbuffer);
  glVertexAttribPointer(
    1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
    2,                                // size : U+V => 2
    GL_FLOAT,                         // type
    GL_FALSE,                         // normalized?
    0,                                // stride
    (void*)0                          // array buffer offset
    );

  // Model matrix : an identity matrix (model will be at the origin)
  //glm::mat4 Position = glm::mat4(1.0f, 0.f, 0.f, 0.0f,
  //  0.0f, 1.f, 0.f, 0.0f,
  //  0.0f, 0.f, 1.f, 0.0f,
  //  x, y, z, 1.0f);
  for (unsigned i = 0; i < mComponents_.size(); ++i)
  {
    auto iter = mComponents_[i];
    if (iter->mParent()->dead){
      mComponents_[i]->clean = true;
      mComponents_.erase(mComponents_.begin() + i);
      --i;
      continue;
    }

    TransformComponent * t = iter->mParent()->GetComponent(TransformComponent);
    glm::mat4 Position;
    Position[3][0] = t->mPositionX();
    Position[3][1] = t->mPositionY();
    Position[3][2] = t->mPositionZ();
    //t->mPositionX(t->mPositionX() + 0.34f);
    //if (t->mPositionX() > 7){
    //  t->mPositionX(-7);
    //}
    glm::mat4 Scale, Rotation;

    Scale[0][0] = t->mScaleX();
    Scale[1][1] = t->mScaleY();
    Scale[2][2] = t->mScaleZ();
    //Scale[1][1] = x * 1.5;
    Rotation = setUpRotationMatrix(Rotation, t->mRotationX(), 1, 0, 0);
    Rotation = setUpRotationMatrix(Rotation, t->mRotationY(), 0, 1, 0);
    Rotation = setUpRotationMatrix(Rotation, t->mRotationZ(), 0, 0, 1);
    // Our ModelViewProjection : multiplication of our 3 matrices
    //glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform

    MatrixID = glGetUniformLocation(programID, "Position");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Position[0][0]);
    MatrixID = glGetUniformLocation(programID, "Scale");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Scale[0][0]);
    MatrixID = glGetUniformLocation(programID, "Rotation");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Rotation[0][0]);
    if (iter->mType() == GraphicsType::SPRITE)
    {
      GLuint tex = mTextureMap_[static_cast<SpriteComponent *>(iter)->mTextureName];
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, tex);

      // Set our "myTextureSampler" sampler to user Texture Unit 0
      glUniform1i(mTextureID, 0);
    }
    

    glUniform4f(ColorID, 1, 0, 0 , 0.5f);

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 3 indices starting at 0 -> 1 triangle
  }
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  //Draw imgui stuff
  //bool show_test_window = true;
  //bool show_another_window = false;
  //
  ImGui_ImplGlfwGL3_NewFrame();
  auto * editor = gCore->GetSystem(EditorSystem);
  editor->DisplayImGUI();

  ImGui::Render();
  // Swap buffers
  glfwSwapBuffers(mWindow);
  glfwPollEvents();
}

void GraphicsSystem::Shutdown()
{
  // Cleanup VBO and shader
  glDeleteBuffers(1, &mVertexbuffer);
  glDeleteProgram(programID);
  glDeleteVertexArrays(1, &mVertexArrayID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();
}

void GraphicsSystem::RegisterComponent(GraphicsComponent * comp)
{
  comp->clean = false;
  mComponents_.push_back(comp);
}

/*
TextureInfo tex;
glGenTextures(1, &tex.texID);
glActiveTexture(GL_TEXTURE0);
// bind the texture to the ID

glBindTexture(GL_TEXTURE_2D, tex.texID);
// gather the data in an array to be past to openGL
unsigned char *image = SOIL_load_image(temp.c_str(), &tex.width, &tex.height, 0, SOIL_LOAD_RGBA);
// pass this data to texID_
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width, tex.height, 0, GL_RGBA,
GL_UNSIGNED_BYTE, image);
// free the image back to the system, it has been added to the texture
SOIL_free_image_data(image);
// stop trying to bind to something.
glBindTexture(GL_TEXTURE_2D, 0);
*/

void GraphicsSystem::LoadTexture(std::string filename)
{
  int w, h;
  GLuint texID;
  // gather the data in an array to be past to openGL
  unsigned char *image = nullptr;
  image = SOIL_load_image(filename.c_str(), &w, &h, 0, SOIL_LOAD_RGBA);
  if (!image)
  {
    std::cout << "YA FUCKD UP" << std::endl;
  }
  // pass this data to texID_
  glGenTextures(1, &texID);
  glActiveTexture(GL_TEXTURE0);
  // bind the texture to the ID

  glBindTexture(GL_TEXTURE_2D, texID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
    GL_UNSIGNED_BYTE, image);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  // free the image back to the system, it has been added to the texture
  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  //
  //unsigned char * p = SOIL_load_image(filename.c_str(), &w, &h, &c, SOIL_LOAD_L);
  //
  ////GLuint texID = SOIL_load_OGL_texture("A.png",
  ////  SOIL_LOAD_AUTO,
  ////  SOIL_CREATE_NEW_ID,
  ////  SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  ////  );
  //texID = SOIL_load_OGL_texture_from_memory
  //  (
  //  p,
  //  w*h*c,
  //  SOIL_LOAD_AUTO,
  //  SOIL_CREATE_NEW_ID,
  //  SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT
  //  );
  if (texID > 0)
  {
    mTextureMap_.insert({ filename, texID });
  }
  else
  {
    std::cout << "Texture loading failed:\n Filename: '"<<filename<<"\nReason: '" << SOIL_last_result() << "'" << std::endl;
  }

}

GLuint GraphicsSystem::GetTexture(std::string textureName)
{
  auto tex = mTextureMap_.find(textureName);
  if (tex != mTextureMap_.end())
    return tex->second;
  return 0; 
}
