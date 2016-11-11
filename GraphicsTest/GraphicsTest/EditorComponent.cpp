#include "EditorComponent.h"
#include "EditorSystem.h"
#include "InputSystem.h"
#include "Core.h"
#include "Globals.h"
#include "TransformComponent.h"
#include "Object.h"

EditorComponent::EditorComponent() : tile(false){
  EditorSystem * g = gCore->GetSystem(EditorSystem);
  g->RegisterComponent(this);

  mName_ = "EditorComponent";
}
bool EditorComponent::Initialize(){
  clean = true;
  return true;
}
void EditorComponent::Update(double dt){
  auto * editor = gCore->GetSystem(EditorSystem);
  if ((tile ^ editor->tileEditorActive)){
    return;
  }
  auto * input = gCore->GetSystem(InputSystem);
  auto * trans = mParent_->GetComponent(TransformComponent);
  double mX = input->GetMouseX();
  double mY = input->GetMouseY();
  if (mX >= trans->mPositionX() - trans->mScaleX() / 2.f && mX <= trans->mPositionX() + trans->mScaleX() / 2.f
    && mY >= trans->mPositionY() - trans->mScaleY() / 2.f && mY <= trans->mPositionY() + trans->mScaleY() / 2.f){
    if ((input->isKeyPressed(GLFW_MOUSE_BUTTON_1) && editor->tileEditorActive) || (input->isKeyJustPressed(GLFW_MOUSE_BUTTON_1) && !editor->tileEditorActive && !tile)){
        editor->Select(mParent_);
    }
    else if ((input->isKeyPressed(GLFW_MOUSE_BUTTON_2) && editor->tileEditorActive) || (input->isKeyJustPressed(GLFW_MOUSE_BUTTON_2) && !editor->tileEditorActive && !tile)){
      mParent_->Destroy();
      editor->Select(nullptr);
    }
  }
}
void EditorComponent::Shutdown(){

}