#include "EditorComponent.h"
#include "EditorSystem.h"
#include "InputSystem.h"
#include "Core.h"
#include "Globals.h"
#include "TransformComponent.h"
#include "Object.h"

EditorComponent::EditorComponent(){
  EditorSystem * g = gCore->GetSystem(EditorSystem);
  g->RegisterComponent(this);

  mName_ = "EditorComponent";
}
bool EditorComponent::Initialize(){
  return true;
}
void EditorComponent::Update(double dt){
  auto * input = gCore->GetSystem(InputSystem);
  auto * trans = mParent_->GetComponent(TransformComponent);
  double mX = input->GetMouseX();
  double mY = input->GetMouseY();
  if (input->isKeyJustPressed(GLFW_MOUSE_BUTTON_1)){
    
    if (mX > trans->mPositionX() - trans->mScaleX() / 2.f && mX < trans->mPositionX() + trans->mScaleX() / 2.f
      && mY > trans->mPositionY() - trans->mScaleY() / 2.f && mY < trans->mPositionY() + trans->mScaleY() / 2.f){
      std::cout << mX << "," << mY << std::endl;
      std::cout << trans->mPositionX() - trans->mScaleX() / 2.f << "," << trans->mPositionX() + trans->mScaleX() / 2.f << std::endl;
      std::cout << trans->mPositionY() - trans->mScaleY() / 2.f << "," << trans->mPositionY() + trans->mScaleY() / 2.f << std::endl << std::endl;
      auto * editor = gCore->GetSystem(EditorSystem);
      editor->Select(mParent_);
    }
  }
  else if (input->isKeyJustPressed(GLFW_MOUSE_BUTTON_2)){
    
    if (mX > trans->mPositionX() - trans->mScaleX() / 2.f && mX < trans->mPositionX() + trans->mScaleX() / 2.f
      && mY > trans->mPositionY() - trans->mScaleY() / 2.f && mY < trans->mPositionY() + trans->mScaleY() / 2.f){
      std::cout << mX << "," << mY << std::endl;
      std::cout << trans->mPositionX() << "," << trans->mPositionY() << std::endl << std::endl;
      mParent_->Destroy();
      auto * editor = gCore->GetSystem(EditorSystem);
      editor->Select(nullptr);
    }
  }
}
void EditorComponent::Shutdown(){

}