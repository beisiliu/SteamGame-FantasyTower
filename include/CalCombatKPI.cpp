#include "CalCombatKPI.h"

int CalCombatKPI::calNormalAttackAndDefense(int health, int attack, int defense)
{
	if (attack > defense)
	{
		health = health - (attack - defense);
		if (health >= 0) { return health; }
		else { return 0; }
	}
	else {
		return health;
	}

}