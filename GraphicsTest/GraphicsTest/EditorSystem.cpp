#include "EditorSystem.h"
#include "Object.h"
#include "InputSystem.h"
#include "Core.h"
#include "Globals.h"
#include "EditorComponent.h"
#include "ObjectSystem.h"
#include "JSONTranslator.h"
#include <imgui.h>

EditorSystem::EditorSystem(){
  mName_ = "EditorSystem";
}


bool EditorSystem::mCreateObjectMenu;
void EditorSystem::RegisterComponent(EditorComponent*comp){
  mComponents_.push_back(comp);
}
bool EditorSystem::Initialize(){
  showfilter = false;
  actions.push_back({ "Create Object", EditorSystem::SetCreateObj });
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
  }
  if (!gCore->editor)return;
  if (input->isKeyJustPressed(GLFW_MOUSE_BUTTON_1)){
    selected = nullptr;
  }
  
  if (input->isKeyJustPressed(GLFW_KEY_SPACE) && !showfilter){
    showfilter = true;
  }
  for (unsigned i = 0; i < mComponents_.size(); ++i)
  {
    auto iter = mComponents_[i];
    if (iter->mParent()->dead){
      mComponents_[i]->clean = true;
      mComponents_.erase(mComponents_.begin() + i);
      --i;
      continue;
    }
    
    mComponents_[i]->Update(dt);
  }
  if (input->isKeyPressed(GLFW_MOUSE_BUTTON_1) && selected != nullptr){
    selected->GetComponent(TransformComponent)->mPosition(input->GetMouseX(), input->GetMouseY(), selected->GetComponent(TransformComponent)->mPositionZ());
  }

  //if (input->isKeyJustPressed(GLFW_MOUSE_BUTTON_1) && selected == nullptr){
  //  JSONTranslator j;
  //  Object * b;
  //  b = j.CreateObjectFromFile("B.json");
  //  b->name = "Fuccboi2";
  //  auto * trans2 = b->GetComponent(TransformComponent);
  //  trans2->mPosition_.y = input->GetMouseY();
  //  trans2->mPosition_.x = input->GetMouseX();
  //  b->Initialize();
  //}
}
void EditorSystem::Shutdown(){

}

void EditorSystem::Select(Object * obj){
  selected = obj;
}

void EditorSystem::DisplayImGUI(){
  if (gCore->editor == false){
    selected = nullptr;
    return;
  }
  if (selected != nullptr){
    ImGui::Begin("Inspector");
    ImGui::Text(selected->name.c_str());
    for (auto & iter : selected->mComponents){
      if (ImGui::CollapsingHeader(iter.second->mName().c_str())){
        for (auto & memIter : iter.second->members){
          memIter.second->GetUI(memIter.first);
        }
      }
    }
    ImGui::End();
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
}

//ACTIONS DOWN HERE
void EditorSystem::SetCreateObj()
{
  mCreateObjectMenu = true;
}

void EditorSystem::CreateObjectByName(const char * name)
{
  Object * obj;
  
  gCore->GetSystem(ObjectSystem)->CreateObjectFromFile(name);
}