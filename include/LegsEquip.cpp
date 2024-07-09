#include"LegsEquip.h"

std::map<std::string, int> setBeginnerKneepad() 
{
	std::map<std::string, int> BeginnerKneepad;

	BeginnerKneepad["buyGold"] = 10;
	BeginnerKneepad["sellGold"] = 5;
	BeginnerKneepad["defense"] = 1;

	return BeginnerKneepad;
}