#pragma once

class EnemyAttribute {
public:
	EnemyAttribute(int floorNum);
	void setEnemyAttributeFloor001();
	void setEnemyAttributeFloor002();
	void setEnemyAttributeFloor003();
	void setEnemyAttributeFloor004();
	void setEnemyAttributeFloor005();
	void setEnemyAttributeFloor006();  // 六层后，敌人增加一个状态值
	void setEnemyAttributeFloor007();

	int p_enemyAttack;
	int p_enemyDefense;
	int p_enemySpeed;
	int p_enemyHealth; // 可变的
	int p_enemyHealthLimit; // 不变的血量上限
	int p_enemyStatus; // 可变的
	int p_enemeStatusLimit; // 不变的状态上限
};
