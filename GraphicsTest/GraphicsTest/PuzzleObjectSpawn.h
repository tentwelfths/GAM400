#pragma once
#include "GameLogicComponent.h"

class PuzzleObjectSpawn : public GameLogicComponent
{
  public:
    PuzzleObjectSpawn();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
  private:
    int numOfObjects;
    std::vector<Object*> objectsInWorld;
    std::vector<std::string> listOfTextureNames;
    std::vector<glm::vec3> listOfColors;
    std::vector<std::vector<std::string>> hintSpriteList;
    std::vector<std::vector<glm::vec3>> hintColorList;
};