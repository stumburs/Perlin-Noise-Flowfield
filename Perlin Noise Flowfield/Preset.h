#pragma once
#include <fstream>
#include <raylib.h>
#include <unordered_map>
#include <variant>
#include <string>

struct Preset
{
	std::unordered_map<std::string, float> values;
};

Preset ReadPreset(std::string file_name);

void LoadPreset(Preset &preset);

float GetValueAtKey(const Preset& preset, std::string key);

