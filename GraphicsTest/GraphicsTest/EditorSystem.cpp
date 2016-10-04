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
  return true;
}
void EditorSystem::Update(double dt){
  auto * input = gCore->GetSystem(InputSystem);
  if (input->isKeyJustPressed(GLFW_MOUSE_BUTTON_1)){
    selected = nullptr;
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
    b->Initialize();


    auto * trans2 = b->GetComponent(TransformComponent);
    trans2->mPosition_.y = input->GetMouseY();
    trans2->mPosition_.x = input->GetMouseX();
  }
}
void EditorSystem::Shutdown(){

}

void EditorSystem::Select(Object * obj){
  selected = obj;
}

void EditorSystem::DisplayImGUI(){
  if (selected != nullptr){
    ImVec4 clear_color = ImColor(114, 144, 154);
    {
      static float f = 0.0f;
      ImGui::Text(selected->name.c_str());
      for (auto & iter : selected->mComponents){
        if (ImGui::CollapsingHeader(iter.second->mName().c_str())){
          for (auto & memIter : iter.second->members){
            memIter.second->GetUI(memIter.first);
          }
        }
      }
      //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      //ImGui::ColorEdit3("clear color", (float*)&clear_color);
      //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    
    // 2. Show another simple window, this time using an explicit Begin/End pair
   // if (show_another_window)
    //{
    //  ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
    //  ImGui::Begin("Another Window");
    //  ImGui::Text("Hello");
    //  ImGui::End();
    //}
    //
    //// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
    ////if (show_test_window)
    //{
    //  ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
    //  ImGui::ShowTestWindow();
    //}
  }
}