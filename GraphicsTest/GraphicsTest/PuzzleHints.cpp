#include "PuzzleHints.h"
#include "ObjectSystem.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "JSONTranslator.h"
#include "MessagingSystem.h"
#include "Messages.h"

#define NUMOFHINTS 5

PuzzleHints::PuzzleHints() : GameLogicComponent(GameLogicType::PUZZLEHINTS)
{
  mName_ = "PuzzleHints";
}

bool PuzzleHints::Initialize()
{
  JSONTranslator j;

  for (int i = 0; i < 10; ++i)
    mHintFilenames[0][i] = "DistanceHint" +std::to_string(i)+ ".png";
  for (int i = 0; i < 10; ++i)
    mHintFilenames[1][i] = "KnobHint" + std::to_string(i) + ".png";
  for (int i = 0; i < 10; ++i)
    mHintFilenames[2][i] = "PhotocellHint" + std::to_string(i) + ".png";
  for (int i = 0; i < 10; ++i)
    mHintFilenames[3][i] = "SliderHint" + std::to_string(i) + ".png";
  for (int i = 0; i < 10; ++i)
    mHintFilenames[4][i] = "SwitchesHint" + std::to_string(i) + ".png";


  for (int i = 0; i < NUMOFHINTS; ++i)
  {
    int index = (rand() % 10);
    hintsList.push_back(index);

    Object * obj = j.CreateObjectFromFile("Empty.json");
    obj->GetComponent(SpriteComponent)->SetTexture(mHintFilenames[i][index]);
    obj->GetComponent(SpriteComponent)->mTint({ 0, 0, 0 });
    obj->mVisibility[4] = false;
    glm::vec3 newTrans(-7, 34 - 2 * i, 10);
    obj->GetComponent(TransformComponent)->mPosition(newTrans);
    obj->GetComponent(TransformComponent)->mScale(8,2,1);
    obj->save = false;
    obj->Initialize();
    obj->Register();
    hintObjects.push_back(obj);

    if (i == 0)
    {
      Object * bat = j.CreateObjectFromFile("Empty.json");
      bat->GetComponent(SpriteComponent)->SetTexture("Battery.png");
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
      float y = 30.0f + randY;
      glm::vec3 batTrans(randX, y, 9.0);
      bat->GetComponent(TransformComponent)->mPosition(batTrans);
      bat->Initialize();
      bat->Register();
    }
    if (i == 1)
    {
      if (index == 1 || index == 9 ||index == 3)
      {
        Object * star = j.CreateObjectFromFile("Empty.json");
        star->GetComponent(SpriteComponent)->SetTexture("StarSticker.png");
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
        float y = 30.0f + randY;
        glm::vec3 starTrans(randX, y, 9.0);
        star->GetComponent(TransformComponent)->mPosition(starTrans);
        star->Initialize();
        star->Register();
      }
      if (index == 4 || index == 3)
      {
        Object * rainbow = j.CreateObjectFromFile("Empty.json");
        rainbow->GetComponent(SpriteComponent)->SetTexture("RainbowSticker.png");
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
        float y = 30.0f + randY;
        glm::vec3 rainbowTrans(randX, y, 9.0);
        rainbow->GetComponent(TransformComponent)->mPosition(rainbowTrans);
        rainbow->Initialize();
        rainbow->Register();
      }
      if (index == 0 || index == 2 || index == 4 || index == 5 || index == 8)
      {
        Object * awesome = j.CreateObjectFromFile("Empty.json");
        awesome->GetComponent(SpriteComponent)->SetTexture("AwesomeSticker.png");
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
        float y = 30.0f + randY;
        glm::vec3 awesomeTrans(randX, y, 9.0);
        awesome->GetComponent(TransformComponent)->mPosition(awesomeTrans);
        awesome->Initialize();
        awesome->Register();

        Object * cloud = j.CreateObjectFromFile("Empty.json");
        cloud->GetComponent(SpriteComponent)->SetTexture("CloudSticker.png");
        randX = float(rand()) / float(RAND_MAX / 8);
        randY = float(rand()) / float(RAND_MAX / 5);
        negX = rand() % 2;
        if (negX > 0)
        {
          randX = -randX;
        }
        negY = rand() % 2;
        if (negY > 0)
        {
          randY = -randY;
        }
        y = 30.0f + randY;
        glm::vec3 cloudTrans(randX, y, 9.0);
        cloud->GetComponent(TransformComponent)->mPosition(cloudTrans);
        cloud->Initialize();
        cloud->Register();
      }
      else
      {
        float aweCheck = float(rand()) / float(RAND_MAX);
        if (aweCheck >= 0.5)
        {
          Object * awesome = j.CreateObjectFromFile("Empty.json");
          awesome->GetComponent(SpriteComponent)->SetTexture("AwesomeSticker.png");
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
          float y = 30.0f + randY;
          glm::vec3 awesomeTrans(randX, y, 9.0);
          awesome->GetComponent(TransformComponent)->mPosition(awesomeTrans);
          awesome->Initialize();
          awesome->Register();
        }
        else
        {
          float cloudCheck = float(rand()) / float(RAND_MAX);
          if (cloudCheck > 0.5)
          {
            Object * cloud = j.CreateObjectFromFile("Empty.json");
            cloud->GetComponent(SpriteComponent)->SetTexture("CloudSticker.png");
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
            float y = 30.0f + randY;
            glm::vec3 cloudTrans(randX, y, 9.0);
            cloud->GetComponent(TransformComponent)->mPosition(cloudTrans);
            cloud->Initialize();
            cloud->Register();
          }
        }
      }
    }
    if (i == 4)
    {
      auto* check  = j.CreateObjectFromFile("RightStick.json");
      auto* spr = check->GetComponent(SpriteComponent);
      if (index == 9 || index == 2 || index == 1)
      {
        spr->mTint(glm::vec3(1.0f, 0.0f, 0.0f));
      }
      else if (index == 6 || index== 4 || index == 3 || index == 0)
      {
        spr->mTint(glm::vec3(0.0f, 1.0f, 0.0f));
      }
      else
      {
        spr->mTint(glm::vec3(0.0f, 0.0f, 1.0f));
      }
      spr->hasChanged = true;
      check->GetComponent(TransformComponent)->mPositionZ(5);
      check->Initialize();
      check->Register();
    }

    hintsRevealed.push_back(false);
  }
  return true;
}

void PuzzleHints::Update(double dt)
{

}

void PuzzleHints::Shutdown()
{

}

void PuzzleHints::SetHintRevealed(int hintNum)
{
  hintsRevealed[hintNum] = true;
  for (int i = 0; i < 5; ++i)
  {
    hintObjects[hintNum]->mVisibility[i] = true;
  }
  hintObjects[hintNum]->hasChanged = true;
}

bool PuzzleHints::GetHintRevealed(int hintNum)
{
  return hintsRevealed[hintNum];
}

int PuzzleHints::GetValue(int valuePos)
{
  return hintsList[valuePos];
}