#pragma once
#include "Metadata.h"

class Core;

extern std::unordered_map<std::string, IComponent*()> components;
extern Core * gCore;