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
  float mRotDelta, mPosDelta, mScaleDelta;
private:
  Object * selected;
  std::vector<EditorComponent *> mComponents_;
  bool showfilter;
  std::vector <std::pair< std::string, void(*)()>> actions;
  static void SetCreateObj();
  static void SetSaveLevel();
  static void SetLoadLevel();
  static void SetTileEditor();
  static void ReloadArtAssets();
  static void ShowDeltaMenu();
  void CreateObjectByName(const char * name);
  static bool mCreateObjectMenu;
  static bool mSaveLevelMenu;
  static bool mLoadLevelMenu;
  static bool mTileEditorStart;
  static bool mShowDeltaMenu;
  std::string tileArchetype;
  bool mShowAddComponent;
  bool mShowSaveArchetype;
  
  char str[64];
  char tool;
  float offsetx;
  float offsety;
};