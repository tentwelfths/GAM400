#include "EditorSystem.h"
#include "Object.h"
#include "InputSystem.h"
#include "Core.h"
#include "Globals.h"
#include "EditorComponent.h"
#include "ObjectSystem.h"
#include "GraphicsSystem.h"
#include "JSONTranslator.h"
#include <imgui.h>
#include <math.h>   

EditorSystem::EditorSystem() : showfilter(false), mShowAddComponent(false)
{
  str[0] = 0;
  mName_ = "EditorSystem";
  tileEditorActive = false;
}


bool EditorSystem::mCreateObjectMenu = false;
bool EditorSystem::mSaveLevelMenu = false;
bool EditorSystem::mLoadLevelMenu = false;
bool EditorSystem::mTileEditorStart = false;
void EditorSystem::RegisterComponent(EditorComponent*comp){
  mComponents_.push_back(comp);
}
bool EditorSystem::Initialize(){
  showfilter = false;
  actions.push_back({ "Create Object", EditorSystem::SetCreateObj });
  actions.push_back({ "Load Level", EditorSystem::SetLoadLevel });
  actions.push_back({ "Save Level", EditorSystem::SetSaveLevel });
  actions.push_back({ "Tile Editor", EditorSystem::SetTileEditor });
  return true;
}
void EditorSystem::Update(double dt){
  
  auto * input = gCore->GetSystem(InputSystem);
  //if (input->isKeyJustPressed(GLFW_KEY_O)){
  //  gCore->LoadLevel("DemoLevel.json");
  //}
  //else if (input->isKeyJustPressed(GLFW_KEY_P)){
  //  gCore->UnloadLevel();
  //}
  if (input->isKeyJustPressed(GLFW_KEY_GRAVE_ACCENT)){
    gCore->editor = !gCore->editor;
    tileEditorActive = false;
  }
  if (!gCore->editor)return;
  if ((input->isKeyPressed(GLFW_MOUSE_BUTTON_1) && tileEditorActive) || (input->isKeyJustPressed(GLFW_MOUSE_BUTTON_1) && !tileEditorActive)){
    selected = nullptr;
  }
  
  if (tileEditorActive){
    if (input->isKeyJustPressed(GLFW_KEY_ESCAPE)){
      tileEditorActive = false;
      tileArchetype = "";
    }
    if (input->isKeyJustPressed(GLFW_KEY_T)){
      tileEditorActive = false;
      tileArchetype = "";
      mTileEditorStart = true;
    }
  }

  if (input->isKeyJustPressed(GLFW_KEY_SPACE) && !showfilter){
    showfilter = true;
    mShowAddComponent = false;
    mCreateObjectMenu = false;
    mLoadLevelMenu = false;
    mSaveLevelMenu = false;
    tileEditorActive = false;
    selected = nullptr;
  }
  for (unsigned i = 0; i < mComponents_.size(); ++i)
  {
    auto iter = mComponents_[i];
    if (iter->dead){
      mComponents_[i]->clean = true;
      mComponents_.erase(mComponents_.begin() + i);
      --i;
      continue;
    }
    
    mComponents_[i]->Update(dt);
  }
  if (input->isKeyPressed(GLFW_MOUSE_BUTTON_1) && selected != nullptr && !tileEditorActive){
    float x = input->GetMouseX(), y = input->GetMouseY(), z = selected->GetComponent(TransformComponent)->mPositionZ();
    if (input->isKeyPressed(GLFW_KEY_LEFT_SHIFT) || input->isKeyPressed(GLFW_KEY_RIGHT_SHIFT))
    {
      x = round(x);
      y = round(y);
    }
    selected->GetComponent(TransformComponent)->mPosition(x, y, z);
  }

  if (input->isKeyPressed(GLFW_MOUSE_BUTTON_1) && selected == nullptr && tileEditorActive){
    Object * obj = gCore->GetSystem(ObjectSystem)->CreateObjectFromFile(tileArchetype);
    obj->GetComponent(TransformComponent)->mPosition(round(input->GetMouseX()), round(input->GetMouseY()), obj->GetComponent(TransformComponent)->mPositionZ());
    obj->GetComponent(TransformComponent)->mScale(1,1,1);
  }

  auto * g = gCore->GetSystem(GraphicsSystem);
  if (input->isKeyPressed(GLFW_KEY_LEFT)){
    g->mMainCamera.x -= g->mMainCamera.zoom / 2.f;
  }
  if (input->isKeyPressed(GLFW_KEY_RIGHT)){
    g->mMainCamera.x += g->mMainCamera.zoom / 2.f;
  }
  if (input->isKeyPressed(GLFW_KEY_DOWN)){
    g->mMainCamera.y -= g->mMainCamera.zoom / 2.f;
  }
  if (input->isKeyPressed(GLFW_KEY_UP)){
    g->mMainCamera.y += g->mMainCamera.zoom / 2.f;
  }
  g->mMainCamera.zoom -= input->GetScrollDelta() * 0.25;

}
void EditorSystem::Shutdown(){

}

void EditorSystem::Select(Object * obj){
  selected = obj;
}

void EditorSystem::DisplayImGUI(){
  if (gCore->editor == false){
    selected = nullptr;
    mShowAddComponent = false;
    showfilter = false;
    mCreateObjectMenu = false;
    mLoadLevelMenu = false;
    mSaveLevelMenu = false;
    str[0] = 0;
    return;
  }
  //bool b = true;
  //ImGui::ShowTestWindow(&b);
  if (selected != nullptr){
    ImGui::Begin("Inspector");
    char name[64];
    strcpy(name, selected->name.c_str());
    ImGui::InputText("Name",name, 64);
    selected->name = name;
    for (auto & iter : selected->mComponents){
      if (ImGui::CollapsingHeader(iter.second->mName().c_str())){
        for (auto & memIter : iter.second->members){
          memIter.second->GetUI(memIter.first);
        }
        if (ImGui::Button(std::string("Remove " + iter.second->mName()).c_str())){
          selected->RemoveComponent(iter.second);
          break;
        }
      }
    }

    if (ImGui::Button(std::string("Add Component").c_str())){
      mShowAddComponent = true;
    }
    if (ImGui::Button(std::string("Save Archetype").c_str())){
      mShowSaveArchetype = true;
    }
    ImGui::End();
  }
  else{
    mShowAddComponent = false;
  }

  if (showfilter){
    //ImGui::ShowTestWindow(&showfilter);
    ImGui::Begin("Filter");
    static ImGuiTextFilter filter;
    filter.Draw();
    //const char* lines[] = { "aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world" };
    for (unsigned  i = 0; i < actions.size(); i++){
      if (filter.PassFilter(actions[i].first.c_str())){
        if (ImGui::Button(actions[i].first.c_str())){
          actions[i].second();
          showfilter = false;
        }
      }
    }
    ImGui::End();
  }
  if (mCreateObjectMenu){
    ImGui::Begin("CreateObjectMenu");
    ImGui::InputText("Archetype Name", str, 64);
    if (ImGui::Button("Create Object")){
      CreateObjectByName(str);
      str[0] = 0;
      mCreateObjectMenu = false;
    }
    ImGui::End();
  }

  if (mLoadLevelMenu){
    ImGui::Begin("Load Level Menu");
    ImGui::InputText("Level Name", str, 64);
    if (ImGui::Button("Load Level")){
      gCore->UnloadLevel();
      gCore->LoadLevel(str);
      str[0] = 0;
      mLoadLevelMenu = false;
      selected = nullptr;
    }
    ImGui::End();
  }
  if (mSaveLevelMenu){
    ImGui::Begin("Save Level Menu");
    ImGui::InputText("Level Name", str, 64);
    if (ImGui::Button("Save Level")){
      gCore->SaveLevel(str);
      str[0] = 0;
      mSaveLevelMenu = false;
      selected = nullptr;
    }
    ImGui::End();
  }
  if (mTileEditorStart){
    ImGui::Begin("Start Tile Editor");
    ImGui::InputText("Tile Archetype", str, 64);
    if (ImGui::Button("Start Tiling!")){
      tileEditorActive = true;
      tileArchetype = str;
      str[0] = 0;
      mTileEditorStart = false;
      selected = nullptr;
    }
    ImGui::End();
  }


  if (mShowAddComponent){
    const char* items[128];
    int i = 0;
    for (auto & iter : components){
      items[i] = iter.first.c_str();
      ++i;
    }
    static int item2 = -1;
    ImGui::Begin("Add Component Menu");
    ImGui::Combo("combo scroll", &item2, items, components.size());   // Combo using proper array. You can also pass a callback to retrieve array value, no need to create/copy an array just for that.
    //ImGui::InputText("Component Name", str, 64);
    if (ImGui::Button("Add Component")){
      selected->AddComponent(items[item2]);
      str[0] = 0;
      mShowAddComponent = false;
    }
    ImGui::End();
  }
  if (mShowSaveArchetype){
    ImGui::Begin("Save Archetype Menu");
    ImGui::InputText("Archetype name", str, 64);
    if (ImGui::Button("Save Archetype")){
      
      JSONTranslator j;
      selected->source = str;
      j.CreateFileFromObject(selected);
      str[0] = 0;
      mShowSaveArchetype = false;
      selected = nullptr;
    }
    ImGui::End();
  }

}

//ACTIONS DOWN HERE
void EditorSystem::SetCreateObj()
{
  mCreateObjectMenu = true;
}
void EditorSystem::SetLoadLevel()
{
  mLoadLevelMenu = true;
}
void EditorSystem::SetSaveLevel()
{
  mSaveLevelMenu = true;
}
void EditorSystem::SetTileEditor()
{
  mTileEditorStart= true;
}



void EditorSystem::CreateObjectByName(const char * name)
{
  Object * obj = gCore->GetSystem(ObjectSystem)->CreateObjectFromFile(name);
  if (obj != nullptr){
    auto * g = gCore->GetSystem(GraphicsSystem);

    obj->GetComponent(TransformComponent)->mPosition(g->mMainCamera.x, g->mMainCamera.y, 0);
  }
}