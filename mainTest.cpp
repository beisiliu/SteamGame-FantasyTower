//#include"include/nlohmann/json.hpp"
//#include<fstream>
//#include<ostream>
//#include<iostream>
//#include<string>
//
//using json = nlohmann::json;
//
//
//int main()
//{
//	std::ifstream test_read("save/save01.bin", std::ios::binary);
//	json j = json::from_bson(test_read);
//	json k = j["Weapons"];
//	// std::cout << j["SchoolButton"]["BaseTrainFrame"].items() << std::endl;
//
//	// ²âÊÔ³É¹¦
//	//for (const auto& [key, value] : k.items()) {
//	//	std::cout << "Key: " << key << ", Value: " << value << std::endl;
//	//}
//
//	for (auto it = k.begin(); it != k.end(); ++it) {
//		std::cout << "Key: " << it.key() << ", Value: " << it.value() << std::endl;
//	}
//
//
//	return 0;
//}