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

struct TextureType
{
  std::string name;
  int frames;
  int rows;
  int cols;
  int index;
  GLuint textureID;
};

class GraphicsSystem : public System
{
public:
  GraphicsSystem();

  bool Initialize();

  void Update(double dt);

  void Shutdown();

  void RegisterComponent(GraphicsComponent * comp);

  GLuint LoadTexture(std::string filename);

  void RegisterTexture(TextureType t);

  GLuint GetTexture(std::string textureName);

  std::string frameData;
  glm::mat4 Projection;
  glm::mat4 View;
  std::unordered_map<std::string, TextureType> mTextureMap_;
private:
  GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

  GLFWwindow * mWindow;
  GLuint mVertexbuffer;
  GLuint mUVbuffer;
  GLfloat mVertexArray[18];
  GLfloat mUVArray[12];
  GLuint mVertexArrayID;
  GLuint programID;
  float width, height;
  //GLuint mTexture;
  GLuint mTextureID;
  std::vector<GraphicsComponent *> mComponents_;
  void GraphicsSystem::GatherFrameData(GraphicsComponent * iter);
};