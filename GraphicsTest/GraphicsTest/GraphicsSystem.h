#pragma once

#include <GL/glew.h>
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "System.h"
#include "Standard.h"

class GraphicsComponent;

class GraphicsSystem : public System
{
public:
  GraphicsSystem();

  bool Initialize();

  void Update(double dt);

  void Shutdown();

  void RegisterComponent(GraphicsComponent * comp);

  void LoadTexture(std::string filename);

  GLuint GetTexture(std::string textureName);

private:
  GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

  GLFWwindow * mWindow;
  GLuint mVertexbuffer;
  GLuint mUVbuffer;
  GLfloat mVertexArray[18];
  GLfloat mUVArray[12];
  GLuint mVertexArrayID;
  GLuint programID;

  GLuint mTexture;
  GLuint mTextureID;
  std::unordered_map<std::string, GLuint> mTextureMap_;
  std::vector<GraphicsComponent *> mComponents_;

};