#pragma once
#include <fstream>
#include <raylib.h>
#include <map>
#include <variant>
#include <string>

struct Preset
{
	//std::map<std::string, std::variant<int, float>> values;
	std::map<std::string, float> values;
};

Preset ReadPreset(std::string file_name);

void LoadPreset(Preset &preset);

