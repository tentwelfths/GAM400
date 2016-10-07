#include "EditorSystem.h"
#include "Object.h"
#include "InputSystem.h"
#include "Core.h"
#include "Globals.h"
#include "EditorComponent.h"
#include "JSONTranslator.h"
#include <imgui.h>

EditorSystem::EditorSystem(){
  mName_ = "EditorSystem";
}

void EditorSystem::RegisterComponent(EditorComponent*comp){
  mComponents_.push_back(comp);
}
bool EditorSystem::Initialize(){
  showfilter = false;
  return true;
}
void EditorSystem::Update(double dt){
  
  auto * input = gCore->GetSystem(InputSystem);
  if (input->isKeyJustPressed(GLFW_KEY_O)){
    gCore->LoadLevel("DemoLevel.json");
  }
  else if (input->isKeyJustPressed(GLFW_KEY_P)){
    gCore->UnloadLevel();
  }
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
  if (input->isKeyJustPressed(GLFW_MOUSE_BUTTON_1) && selected == nullptr){
    JSONTranslator j;
    Object * b;
    b = j.CreateObjectFromFile("B.json");
    b->name = "Fuccboi2";
    auto * trans2 = b->GetComponent(TransformComponent);
    trans2->mPosition_.y = input->GetMouseY();
    trans2->mPosition_.x = input->GetMouseX();
    b->Initialize();
  }
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
}