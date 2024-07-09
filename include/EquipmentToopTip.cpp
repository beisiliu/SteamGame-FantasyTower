#include "EquipmentToolTip.h"

void EquipmentToolTip::equitWeaponsToolTip(std::string weaponsName, json p_settingLanguage)
{
	if (ImGui::IsItemHovered() and weaponsName == BEGINNERSWORD)
	{
		if (ImGui::BeginTooltip())
		{
			ImGui::Text(removeQuotes(p_settingLanguage["EquipToolTip"]["Weapons"]["BeginnerSword"].dump()).c_str());
			ImGui::EndTooltip();
		}
	}
}

void EquipmentToolTip::equitHeadToolTip(std::string headName, json p_settingLanguage)
{
	if (ImGui::IsItemHovered() and headName == BEGINNERHELMET)
	{
		if (ImGui::BeginTooltip())
		{
			ImGui::Text(removeQuotes(p_settingLanguage["EquipToolTip"]["HeadEquip"]["BeginnerHelmet"].dump()).c_str());
			ImGui::EndTooltip();
		}
	}
}

void EquipmentToolTip::equitArmsToolTip(std::string armsName, json p_settingLanguage)
{
	if (ImGui::IsItemHovered() and armsName == BEGINNERBARCERS)
	{
		if (ImGui::BeginTooltip())
		{
			ImGui::Text(removeQuotes(p_settingLanguage["EquipToolTip"]["ArmsEquip"]["BeginnerBarcers"].dump()).c_str());
			ImGui::EndTooltip();
		}
	}
}

void EquipmentToolTip::equitBodyToolTip(std::string bodyName, json p_settingLanguage)
{
	if (ImGui::IsItemHovered() and bodyName == BEGINNERBREASTPLATE)
	{
		if (ImGui::BeginTooltip())
		{
			ImGui::Text(removeQuotes(p_settingLanguage["EquipToolTip"]["BodyEquip"]["BeginnerBreastplate"].dump()).c_str());
			ImGui::EndTooltip();
		}
	}
}

void EquipmentToolTip::equitLegsToolTip(std::string legsName, json p_settingLanguage)
{
	if (ImGui::IsItemHovered() and legsName == BEGINNERKNEEPAD)
	{
		if (ImGui::BeginTooltip())
		{
			ImGui::Text(removeQuotes(p_settingLanguage["EquipToolTip"]["LegsEquip"]["BeginnerKneepad"].dump()).c_str());
			ImGui::EndTooltip();
		}
	}
}

void EquipmentToolTip::skillsTootTip(std::string skillName, json p_settingLanguage)
{
	if (ImGui::IsItemHovered() and skillName == SKILLDEFENSE)
	{
		if (ImGui::BeginTooltip())
		{
			ImGui::Text(removeQuotes(p_settingLanguage["SkillsToolTip"]["DefenseDes"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["SkillsToolTip"]["DefenseCost"].dump()).c_str());
			ImGui::EndTooltip();
		}
	}
	if (ImGui::IsItemHovered() and skillName == SKILLHEAVYATTACK)
	{
		if (ImGui::BeginTooltip())
		{
			ImGui::Text(removeQuotes(p_settingLanguage["SkillsToolTip"]["HeavyAttackDes"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["SkillsToolTip"]["HeavyAttackCost"].dump()).c_str());
			ImGui::EndTooltip();
		}
	}
}
