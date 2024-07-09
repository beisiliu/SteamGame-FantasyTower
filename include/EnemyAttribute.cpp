#include "EnemyAttribute.h"

EnemyAttribute::EnemyAttribute(int floorNum)
{
	if (floorNum == 1) { setEnemyAttributeFloor001(); }
	if (floorNum == 2) { setEnemyAttributeFloor002(); }
	if (floorNum == 3) { setEnemyAttributeFloor003(); }
	if (floorNum == 4) { setEnemyAttributeFloor004(); }
	if (floorNum == 5) { setEnemyAttributeFloor005(); }
	if (floorNum == 6) { setEnemyAttributeFloor006(); }
	if (floorNum == 7) { setEnemyAttributeFloor007(); }
}

void EnemyAttribute::setEnemyAttributeFloor001()
{
	p_enemyAttack = 10;
	p_enemyDefense = 5;
	p_enemySpeed = 3;
	p_enemyHealth = 20;
	p_enemyHealthLimit = 20;
}

void EnemyAttribute::setEnemyAttributeFloor002()
{
	p_enemyAttack = 11;
	p_enemyDefense = 5;
	p_enemySpeed = 3;
	p_enemyHealth = 20;
	p_enemyHealthLimit = 20;
}

void EnemyAttribute::setEnemyAttributeFloor003()
{
	p_enemyAttack = 11;
	p_enemyDefense = 6;
	p_enemySpeed = 4;
	p_enemyHealth = 20;
	p_enemyHealthLimit = 20;
}

void EnemyAttribute::setEnemyAttributeFloor004()
{
	p_enemyAttack = 12;
	p_enemyDefense = 6;
	p_enemySpeed = 4;
	p_enemyHealth = 21;
	p_enemyHealthLimit = 21;
}

void EnemyAttribute::setEnemyAttributeFloor005()
{
	p_enemyAttack = 13;
	p_enemyDefense = 4;
	p_enemySpeed = 4;
	p_enemyHealth = 22;
	p_enemyHealthLimit = 22;
}

void EnemyAttribute::setEnemyAttributeFloor006()
{
	p_enemyAttack = 15;
	p_enemyDefense = 5;
	p_enemySpeed = 5;
	p_enemyHealth = 23;
	p_enemyHealthLimit = 23;
	p_enemyStatus = 10;
	p_enemeStatusLimit = 10;
}

void EnemyAttribute::setEnemyAttributeFloor007()
{
	p_enemyAttack = 16;
	p_enemyDefense = 6;
	p_enemySpeed = 5;
	p_enemyHealth = 24;
	p_enemyHealthLimit = 24;
	p_enemyStatus = 10;
	p_enemeStatusLimit = 10;
}