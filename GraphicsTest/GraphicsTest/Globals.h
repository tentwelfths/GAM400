#pragma once
#include "Metadata.h"
#include "IComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"


template<typename T>
IComponent * CreateComponent(){
return new T;
}


class Core;

extern std::unordered_map<std::string, IComponent*(*)()> components;
extern Core * gCore;


