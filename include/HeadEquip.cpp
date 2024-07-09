#include "HeadEquip.h"

std::map<std::string, int> setBeginnerHelmet()
{
	std::map<std::string, int> BeginnerHelmet;
	BeginnerHelmet["buyGold"] = 10;
	BeginnerHelmet["sellGold"] = 5;
	BeginnerHelmet["defense"] = 1;

	return BeginnerHelmet;
}
