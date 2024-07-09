#include "WriteData.h"

bool WriteData::writeBsonBin(const char* binPATH, const std::string key, const int value)
{
	std::ifstream test_read(binPATH, std::ios::binary);
	json j = json::from_bson(test_read);

	j[key] = value;

	// ����ȡ����json�ļ�ת��bson��ʽ
	std::vector<std::uint8_t> v_bson = json::to_bson(j);

	std::ofstream f(binPATH, std::ios::binary);
	for (std::vector<std::uint8_t>::iterator it = v_bson.begin(); it != v_bson.end(); ++it)
	{
		f << *it;
	}
	f.close();	
	return true;
}

bool WriteData::writeBsonBin2(const char* binPATH, const std::string key1, const std::string key2, const int value)
{
	std::ifstream test_read(binPATH, std::ios::binary);
	json j = json::from_bson(test_read);

	j[key1][key2] = value;

	// ����ȡ����json�ļ�ת��bson��ʽ
	std::vector<std::uint8_t> v_bson = json::to_bson(j);

	std::ofstream f(binPATH, std::ios::binary);
	for (std::vector<std::uint8_t>::iterator it = v_bson.begin(); it != v_bson.end(); ++it)
	{
		f << *it;
	}
	f.close();
	return true;
}


bool WriteData::writeBsonBin(const char* binPATH, const std::string key, bool value, bool useTrue)
{
	std::ifstream test_read(binPATH, std::ios::binary);
	json j = json::from_bson(test_read);

	j[key] = value;

	// ����ȡ����json�ļ�ת��bson��ʽ
	std::vector<std::uint8_t> v_bson = json::to_bson(j);

	std::ofstream f(binPATH, std::ios::binary);
	for (std::vector<std::uint8_t>::iterator it = v_bson.begin(); it != v_bson.end(); ++it)
	{
		f << *it;
	}
	f.close();
	return true;
}

bool WriteData::writeBsonBin(const char* binPATH, const std::string key, const std::string value)
{
	std::ifstream test_read(binPATH, std::ios::binary);
	json j = json::from_bson(test_read);

	j[key] = value;

	// ����ȡ����json�ļ�ת��bson��ʽ
	std::vector<std::uint8_t> v_bson = json::to_bson(j);

	std::ofstream f(binPATH, std::ios::binary);
	for (std::vector<std::uint8_t>::iterator it = v_bson.begin(); it != v_bson.end(); ++it)
	{
		f << *it;
	}
	f.close();
	return true;
}

// ����һ�������ֶ����½���Ϸ��ʱ�����
// �ֶ�����"PlayerName"
void WriteData::initPersonAttributes(const char* binPATH)
{
	json j;
	// ����ȫ��
	j["HtFloor"] = 1;				// ��Ϸ����
	j["gameTime"] = 1;				// ��Ϸʱ��

	// ������Ϣ
	j["personAllHealth"] = 20;      // Ѫ������ֵ
	j["personCurrentHealth"] = 20;  // ��ǰѪ��
	j["personAllStatus"] = 50;		// ״̬����ֵ
	j["personCurrentStatus"] = 50;	// ��ǰ״̬
	j["psersonStrength"] = 10;	// ��������
	j["personDefense"] = 5;		// �������
	j["personSpeed"] = 3;		// �����ٶ�
	j["personMoney"] = 0;		// �����ʼ���Ĭ��0��������ʱ�������޸�
	
	// ����ս�����
	j["isPersonWin"] = false;   // �洢��ң�HT��������Ӯ״̬
	j["isPersonLose"] = false;  // �洢��ң�HT��������Ӯ״̬
	j["isOnceAward"] = false;  // �洢��ң�����Ļ�ȡ������״̬
	
	// ���ڼ�԰ϵͳ
	j["homeSleepAddHealth"] = 20; // ˯����Ѫ20

	// ����ѧԺϵͳ
	j["schoolJuniorStrengthNums"] = 0; // ��¼����ѵ������
	j["schoolJuniorDefenseNums"] = 0; // ��¼����ѵ������
	j["schoolJuniorSpeedNums"] = 0; // ��¼�ٶ�ѵ������

	// ���ؽ���
	j["isGetFloorThreeAward"] = false;

	// �Ƿ�ȡ�óɾ�
	j["isAchievementsFloor001"] = false;
	j["isAchievementsFloor002"] = false;
	j["isAchievementsFloor003"] = false;
	j["isAchievementsFloor004"] = false;
	j["isAchievementsFloor005"] = false;
	j["isAchievementsFloor006"] = false;

	// �Ƿ���ʾ������Ϣ
	j["isEnemyInformaion"] = true;
	// ��¼7�㱦���Ƿ񱻴�
	j["isOpenChests_7floor"] = false;

	// ����ʼ����ֵ����bson�ļ�
	std::vector<std::uint8_t> v_bson = json::to_bson(j);

	// д���ļ�
	std::ofstream f(binPATH, std::ios::binary);
	for (std::vector<std::uint8_t>::iterator it = v_bson.begin(); it != v_bson.end(); ++it)
	{
		f << *it;
	}
	f.close();

}
