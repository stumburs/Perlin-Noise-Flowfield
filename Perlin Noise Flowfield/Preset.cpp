#include "Preset.h"
#include <iostream>

Preset ReadPreset(std::string file_name)
{
	std::ifstream preset_file(file_name);
	std::string line;
	Preset preset;

	if (preset_file.is_open())
	{
		while (std::getline(preset_file, line))
		{
			std::string setting_name = line.substr(0, line.find_first_of(' '));
			std::string setting_value = line.substr(line.find_first_of(' ') + 1);
			preset.values.insert({ setting_name, std::stof(setting_value)});
		}
		preset_file.close();
	}
	return preset;
}


void LoadPreset(Preset &preset)
{
	
}

float GetValueAtKey(const Preset& preset, std::string key)
{
	return preset.values.find(key)->second;
}