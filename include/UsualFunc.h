#pragma once

#include <string>
#include <fstream>  
#include <iostream>  
#include <vector>
#include <cstdlib>
#include <cwchar>

std::string removeQuotes(const std::string& str);

void copyFile(const std::string& source, const std::string& destination);

void removeFile(const std::string& file);

void renameFile(const wchar_t* oldname, const wchar_t* newname);