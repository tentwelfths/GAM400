#pragma once
#include "System.h"

class EditorComponent;
class Object;

class EditorSystem : public System
{
public:
  EditorSystem();
  virtual void RegisterComponent(EditorComponent*);
  virtual bool Initialize();
  virtual void Update(double dt);
  virtual void Shutdown();
  void Select(Object * obj);
  void DisplayImGUI();
  bool tileEditorActive;
private:
  Object * selected;
  std::vector<EditorComponent *> mComponents_;
  bool showfilter;
  std::vector <std::pair< std::string, void(*)()>> actions;
  static void SetCreateObj();
  static void SetSaveLevel();
  static void SetLoadLevel();
  static void SetTileEditor();
  void CreateObjectByName(const char * name);
  static bool mCreateObjectMenu;
  static bool mSaveLevelMenu;
  static bool mLoadLevelMenu;
  static bool mTileEditorStart;
  std::string tileArchetype;
  bool mShowAddComponent;
  char str[64];
};