// �޸�bson�ļ���vlanֵ

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
	void initPersonAttributes(const char* binPATH); //��ʼ���������ԣ����½���Ϸ�浵�ļ�
};

