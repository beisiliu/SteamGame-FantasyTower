#include"UsualFunc.h"

std::string removeQuotes(const std::string& str)
{
	std::string result = str;
	// 去掉开头的双引号    
	if (!result.empty() && result.front() == '"')
	{
		result.erase(0, 1);
	}

	// 去掉末尾的双引号    
	if (!result.empty() && result.back() == '"')
	{
		result.pop_back();
	}

	const char* resultFinal = result.c_str();

	return result;
}

void copyFile(const std::string& source, const std::string& destination)
{
    std::ifstream src(source, std::ios::binary);
    if (!src) std::cerr << "Unable to open source file: " << source << std::endl;

    std::ofstream dst(destination, std::ios::binary);
    if (!dst) {
        std::cerr << "Unable to open destination file: " << destination << std::endl;
        src.close();
    }

    // 使用缓冲区来提高效率  
    std::vector<char> buffer(4096); // 例如，4KB的缓冲区  
    while (src.good()) {
        src.read(buffer.data(), buffer.size());
        dst.write(buffer.data(), src.gcount()); // gcount()返回实际读取的字节数  
    }

    src.close();
    dst.close();
}

void removeFile(const std::string& file)
{
    std::remove(file.c_str());
}

void renameFile(const wchar_t* oldname, const wchar_t* newname)
{
    _wrename(oldname, newname);
}