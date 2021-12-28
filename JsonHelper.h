#pragma once
#include <string>
#include <fstream>
class JsonHelper
{
private:
	std::string m_jsonData;
	int m_status;
	int ValidateJson(std::string jsonData);
public:
	JsonHelper(std::ifstream *jsonStream);
	int GetStatus();
	std::string Get(std::string val);

};

