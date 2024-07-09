#include"bodyEquip.h"

std::map<std::string, int> setBeginnerBreastplate()
{
	std::map<std::string, int> BeginnerBreastplate;

	BeginnerBreastplate["buyGold"] = 20;
	BeginnerBreastplate["sellGold"] = 10;
	BeginnerBreastplate["defense"] = 2;

	return BeginnerBreastplate;
}