#pragma once

class EnemyAttribute {
public:
	EnemyAttribute(int floorNum);
	void setEnemyAttributeFloor001();
	void setEnemyAttributeFloor002();
	void setEnemyAttributeFloor003();
	void setEnemyAttributeFloor004();
	void setEnemyAttributeFloor005();
	void setEnemyAttributeFloor006();  // ����󣬵�������һ��״ֵ̬
	void setEnemyAttributeFloor007();

	int p_enemyAttack;
	int p_enemyDefense;
	int p_enemySpeed;
	int p_enemyHealth; // �ɱ��
	int p_enemyHealthLimit; // �����Ѫ������
	int p_enemyStatus; // �ɱ��
	int p_enemeStatusLimit; // �����״̬����
};
