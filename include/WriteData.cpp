#include "WriteData.h"

bool WriteData::writeBsonBin(const char* binPATH, const std::string key, const int value)
{
	std::ifstream test_read(binPATH, std::ios::binary);
	json j = json::from_bson(test_read);

	j[key] = value;

	// 将读取到的json文件转成bson格式
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

	// 将读取到的json文件转成bson格式
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

	// 将读取到的json文件转成bson格式
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

	// 将读取到的json文件转成bson格式
	std::vector<std::uint8_t> v_bson = json::to_bson(j);

	std::ofstream f(binPATH, std::ios::binary);
	for (std::vector<std::uint8_t>::iterator it = v_bson.begin(); it != v_bson.end(); ++it)
	{
		f << *it;
	}
	f.close();
	return true;
}

// 还有一个姓名字段在新建游戏的时候添加
// 字段名："PlayerName"
void WriteData::initPersonAttributes(const char* binPATH)
{
	json j;
	// 用于全局
	j["HtFloor"] = 1;				// 游戏层数
	j["gameTime"] = 1;				// 游戏时间

	// 人物信息
	j["personAllHealth"] = 20;      // 血量上限值
	j["personCurrentHealth"] = 20;  // 当前血量
	j["personAllStatus"] = 50;		// 状态上限值
	j["personCurrentStatus"] = 50;	// 当前状态
	j["psersonStrength"] = 10;	// 人物力量
	j["personDefense"] = 5;		// 人物防御
	j["personSpeed"] = 3;		// 人物速度
	j["personMoney"] = 0;		// 人物初始金币默认0，做测试时，可以修改
	
	// 用于战斗面板
	j["isPersonWin"] = false;   // 存储玩家，HT层数的输赢状态
	j["isPersonLose"] = false;  // 存储玩家，HT层数的输赢状态
	j["isOnceAward"] = false;  // 存储玩家，各层的获取奖励的状态
	
	// 用于家园系统
	j["homeSleepAddHealth"] = 20; // 睡觉回血20

	// 用于学院系统
	j["schoolJuniorStrengthNums"] = 0; // 记录力量训练次数
	j["schoolJuniorDefenseNums"] = 0; // 记录防御训练次数
	j["schoolJuniorSpeedNums"] = 0; // 记录速度训练次数

	// 隐藏奖励
	j["isGetFloorThreeAward"] = false;

	// 是否取得成就
	j["isAchievementsFloor001"] = false;
	j["isAchievementsFloor002"] = false;
	j["isAchievementsFloor003"] = false;
	j["isAchievementsFloor004"] = false;
	j["isAchievementsFloor005"] = false;
	j["isAchievementsFloor006"] = false;

	// 是否显示敌人信息
	j["isEnemyInformaion"] = true;
	// 记录7层宝箱是否被打开
	j["isOpenChests_7floor"] = false;

	// 将初始化的值存入bson文件
	std::vector<std::uint8_t> v_bson = json::to_bson(j);

	// 写入文件
	std::ofstream f(binPATH, std::ios::binary);
	for (std::vector<std::uint8_t>::iterator it = v_bson.begin(); it != v_bson.end(); ++it)
	{
		f << *it;
	}
	f.close();

}
