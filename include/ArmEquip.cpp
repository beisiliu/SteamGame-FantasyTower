#include"ArmEquip.h"

std::map<std::string, int> setBeginnerBarces()
{
	std::map<std::string, int> BeginnerBarces;

	BeginnerBarces["buyGold"] = 10;
	BeginnerBarces["sellGold"] = 5;
	BeginnerBarces["defense"] = 1;

	return BeginnerBarces;
}