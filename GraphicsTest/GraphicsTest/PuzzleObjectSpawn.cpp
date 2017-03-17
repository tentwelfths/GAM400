#include <Box2D\Box2D.h>
#include "PuzzleObjectSpawn.h"
#include "JSONTranslator.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "PuzzleObject.h"
#include "SpriteComponent.h"
#include "GraphicsSystem.h"

#define NUMOFMODULES 5
#define NUMOFCOLORS 8

PuzzleObjectSpawn::PuzzleObjectSpawn() : GameLogicComponent(GameLogicType::PUZZLESPAWN), numOfObjects(0)
{
  AddMember(PuzzleObjectSpawn, numOfObjects);
  mName_ = "PuzzleObjectSpawn";
}

bool PuzzleObjectSpawn::Initialize()
{
  JSONTranslator j;
  auto * g = gCore->GetSystem(GraphicsSystem);
  for (auto& iter : g->mTextureMap_)
  {
    listOfTextureNames.push_back(iter.first);
  }
  //Init list of colors, then the hint object sprites and colors
  listOfColors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
  listOfColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
  listOfColors.push_back(glm::vec3(0.0f, 0.3f, 0.0f));
  listOfColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
  listOfColors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
  listOfColors.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
  listOfColors.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
  listOfColors.push_back(glm::vec3(1.0f, 0.5f, 0.0f));
  for (int i = 0; i < 5; ++i)
  {
    std::vector<glm::vec3> nullColors;
    hintColorList.push_back(nullColors);
    std::vector<std::string> nullSprite;
    hintSpriteList.push_back(nullSprite);
  }
  hintColorList[0].push_back(glm::vec3(1.0f, 0.5f, 0.0f));
  hintColorList[0].push_back(glm::vec3(1.0f, 1.0f, 0.0f));
  hintColorList[0].push_back(glm::vec3(0.5f, 0.5f, 0.5f));
  hintColorList[0].push_back(glm::vec3(1.0f, 0.0f, 0.0f));
  hintColorList[0].push_back(glm::vec3(0.0f, 0.3f, 0.0f));
  hintColorList[1].push_back(glm::vec3(1.0f, 0.0f, 1.0f));
  hintColorList[1].push_back(glm::vec3(1.0f, 0.0f, 0.0f));
  hintColorList[1].push_back(glm::vec3(1.0f, 0.5f, 0.0f));
  hintColorList[1].push_back(glm::vec3(0.0f, 0.0f, 1.0f));
  hintColorList[1].push_back(glm::vec3(0.0f, 0.0f, 0.0f));
  hintColorList[2].push_back(glm::vec3(0.5f, 0.5f, 0.5f));
  hintColorList[2].push_back(glm::vec3(0.0f, 0.0f, 1.0f));
  hintColorList[2].push_back(glm::vec3(1.0f, 0.0f, 0.0f));
  hintColorList[2].push_back(glm::vec3(1.0f, 0.5f, 0.0f));
  hintColorList[2].push_back(glm::vec3(1.0f, 0.0f, 1.0f));
  hintColorList[3].push_back(glm::vec3(1.0f, 0.0f, 1.0f));
  hintColorList[3].push_back(glm::vec3(1.0f, 0.0f, 0.0f));
  hintColorList[3].push_back(glm::vec3(1.0f, 1.0f, 0.0f));
  hintColorList[3].push_back(glm::vec3(0.0f, 0.3f, 0.0f));
  hintColorList[3].push_back(glm::vec3(1.0f, 0.0f, 1.0f));
  hintColorList[4].push_back(glm::vec3(1.0f, 0.5f, 0.0f));
  hintColorList[4].push_back(glm::vec3(0.0f, 0.3f, 0.0f));
  hintColorList[4].push_back(glm::vec3(0.5f, 0.5f, 0.5f));
  hintColorList[4].push_back(glm::vec3(0.0f, 0.0f, 0.0f));
  hintColorList[4].push_back(glm::vec3(0.0f, 0.3f, 0.0f));

  hintSpriteList[0].push_back("Ball.png");
  hintSpriteList[0].push_back("Dinosaur.png");
  hintSpriteList[0].push_back("WBlock.png");
  hintSpriteList[0].push_back("TeddyBear.png");
  hintSpriteList[0].push_back("JBlock.png");
  hintSpriteList[1].push_back("EBlock.png");
  hintSpriteList[1].push_back("OBlock.ong");
  hintSpriteList[1].push_back("Rattle.png");
  hintSpriteList[1].push_back("Pacifier.png");
  hintSpriteList[1].push_back("Train.png");
  hintSpriteList[2].push_back("YBlock.png");
  hintSpriteList[2].push_back("EBlock.png");
  hintSpriteList[2].push_back("Truck.png");
  hintSpriteList[2].push_back("Cube.png");
  hintSpriteList[2].push_back("Dinosaur.png");
  hintSpriteList[3].push_back("LBlock.png");
  hintSpriteList[3].push_back("Ball.png");
  hintSpriteList[3].push_back("TeddyBear.png");
  hintSpriteList[3].push_back("Rattle.png");
  hintSpriteList[3].push_back("Pacifier.png");
  hintSpriteList[4].push_back("NBlock.png");
  hintSpriteList[4].push_back("Truck.png");
  hintSpriteList[4].push_back("Train.png");
  hintSpriteList[4].push_back("Cube.png");
  hintSpriteList[4].push_back("IBlock.png");

  for (int i = 0; i < numOfObjects; ++i)
  {
    auto* obj = j.CreateObjectFromFile("PuzzleObject.json");
    obj->GetComponent(PuzzleObject)->SetHint((0));
    obj->name += "_" + std::to_string(i);
    int randomSprite = rand() % listOfTextureNames.size();
    int randomColor = rand() % listOfColors.size();
    while (listOfTextureNames[randomSprite] == "Arrow.png" || listOfTextureNames[randomSprite] == "Coneman.png")
    {
      randomSprite = rand() % listOfTextureNames.size();
    }
    bool hasChecked = false;
    while (!hasChecked)
    {
      bool allGood = true;
      for (int i = 0; i < hintColorList.size(); ++i)
      {
        for (int j = 0; j < hintColorList[i].size(); ++j)
        {
          if (hintSpriteList[i][j] == listOfTextureNames[randomSprite] && hintColorList[i][j] == listOfColors[randomColor])
          {
            allGood = false;
            randomColor = rand() % listOfColors.size();
            break;
          }
        }
        if (!allGood)
        {
          break;
        }
      }
      if (!allGood)
      {
        continue;
      }
      hasChecked = true;
    }
    obj->GetComponent(SpriteComponent)->SetTexture(listOfTextureNames[randomSprite]);
    obj->GetComponent(SpriteComponent)->mTint(listOfColors[randomColor]);
    float randX = float(rand())/float(RAND_MAX/8);
    float randY = float(rand())/float(RAND_MAX/5);
    int negX = rand()%2;
    if (negX > 0)
    {
      randX = -randX;
    }
    int negY = rand() % 2;
    if (negY > 0)
    {
      randY = -randY;
    }
    glm::vec3 newTrans(randX, randY, obj->GetComponent(TransformComponent)->mPositionZ());
    obj->GetComponent(TransformComponent)->mPosition(newTrans);
    obj->save = false;
    obj->Initialize();
    obj->Register();
    objectsInWorld.push_back(obj);
  }
  for (int i = 0; i < NUMOFMODULES; ++i)
  {
    auto* obj = j.CreateObjectFromFile("PuzzleObject.json");
    int randomHint = (rand() % 10);
    int randomType = (rand() % hintSpriteList.size());
    obj->GetComponent(PuzzleObject)->SetHint(randomHint);
    obj->name = "HintObject_" + std::to_string(i);
    obj->GetComponent(SpriteComponent)->SetTexture(hintSpriteList[i][randomType]);
    int randomColor = rand() % listOfColors.size();
    obj->GetComponent(SpriteComponent)->mTint(hintColorList[i][randomType]);
    float randX = float(rand()) / float(RAND_MAX / 8);
    float randY = float(rand()) / float(RAND_MAX / 5);
    int negX = rand() % 2;
    if (negX > 0)
    {
      randX = -randX;
    }
    int negY = rand() % 2;
    if (negY > 0)
    {
      randY = -randY;
    }
    glm::vec3 newTrans(randX, randY, obj->GetComponent(TransformComponent)->mPositionZ());
    obj->GetComponent(TransformComponent)->mPosition(newTrans);
    obj->save = false;
    obj->Initialize();
    obj->Register();
    objectsInWorld.push_back(obj);
  }
  return true;
}

void PuzzleObjectSpawn::Update(double dt)
{

}

void PuzzleObjectSpawn::Shutdown()
{
  for (int i = 0; i < objectsInWorld.size(); ++i)
  {
    delete objectsInWorld[i];
  }
  objectsInWorld.clear();
}