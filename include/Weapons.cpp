#include "Weapons.h"

std::map<std::string, int> setBeginnerSword()
{
	std::map<std::string, int> BeginnerSwordMap;
	
	BeginnerSwordMap["buyGold"] = 20;
	BeginnerSwordMap["sellGold"] = 10;
	BeginnerSwordMap["attack"] = 1;

	return BeginnerSwordMap;

}