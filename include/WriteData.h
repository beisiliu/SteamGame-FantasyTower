// 修改bson文件的vlan值

#pragma once

#include"nlohmann/json.hpp"
#include<fstream>
#include<ostream>
#include<iostream>
#include<string>


using json = nlohmann::json;

class WriteData
{
public:
	bool writeBsonBin(const char* binPATH, const std::string key, const int value);
	bool writeBsonBin2(const char* binPATH, const std::string key1, const std::string key2, const int value);
	bool writeBsonBin(const char* binPATH, const std::string key, bool value, bool useTrue);
	bool writeBsonBin(const char* binPATH, const std::string key, const std::string value);
	void initPersonAttributes(const char* binPATH); //初始化人物属性，并新建游戏存档文件
};

