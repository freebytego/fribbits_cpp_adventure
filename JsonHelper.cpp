#include "JsonHelper.h"
#include <sstream>

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

JsonHelper::JsonHelper(std::ifstream *_jsonStream) {
	std::ostringstream jsonStream;
	jsonStream << _jsonStream->rdbuf();

	std::string jsonData = jsonStream.str();

	m_status = this->ValidateJson(jsonData);
	if (m_status == -1) {
		this->m_jsonData = "{}";
	}
	else {
		std::ostringstream o;
		for (auto c = jsonData.cbegin(); c != jsonData.cend(); c++) {
			switch (*c) {
			case '"': o << "\\\""; break;
			case '\\': o << "\\\\"; break;
			case '\b': o << "\\b"; break;
			case '\f': o << "\\f"; break;
			case '\n': o << "\\n"; break;
			case '\r': o << "\\r"; break;
			case '\t': o << "\\t"; break;
			default: o << *c;
			}
		}
		this->m_jsonData = o.str();
	}
}

int JsonHelper::ValidateJson(std::string jsonData) {
	if (jsonData.size() >= 2 && // JSON has to be at least 2 symbols long: "{}" <- this is valid
		((jsonData[0] == '{' && jsonData[jsonData.size() - 1] == '}') ||
		(jsonData[0] == '[' && jsonData[jsonData.size() - 1] == ']'))) {
		return 1;
	}
	else {
		return -1;
	}
}

int JsonHelper::GetStatus() {
	return this->m_status;
}

std::string JsonHelper::Get(std::string val) {
	if (m_status == -1) return "";

	std::size_t valuePosition = this->m_jsonData.find("\\\"" + val + "\\\"");
	std::string ret;
	bool foundComma{ false };
	bool hasMoreValues{ true };
	if (valuePosition != std::string::npos) {
		int startPosition = valuePosition + val.size() + 4; // ":" Position
		int endPosition = this->m_jsonData.find(",", startPosition);
		if (endPosition < 0) {
			hasMoreValues = false;
			endPosition = this->m_jsonData.find("}", startPosition);
		}
		for (int offset{ endPosition }; offset < this->m_jsonData.size(); offset++) {
			if (this->m_jsonData[offset] == '"' && this->m_jsonData.substr(offset - 3, 4) != "\\\\\\\"") {
				std::cout << this->m_jsonData[offset - 4];
				std::string rawValueString;
				rawValueString = this->m_jsonData.substr(startPosition + 2, (offset) - (startPosition + 2));
				int commaPosition = rawValueString.find("\\\",");
				if (commaPosition != std::string::npos) {
					foundComma = true;
				}

				if (foundComma) {
					std::cout << "A";
				}

				std::cout << "Raw string: " << ' ' << rawValueString << '\n';
				if (!hasMoreValues) {
					offset++;
				}
				rawValueString = this->m_jsonData.substr(offset + 1, this->m_jsonData.find("\"", offset + 1) - offset + 1);
				endPosition = offset + rawValueString.length() - 1;

#ifdef DEBUG
				std::cout << "Found comma: " << foundComma << '\n'
					<< "Has more values: " << hasMoreValues << '\n'
					<< "Offset: " << offset << '\n'
					<< "End position: " << endPosition << '\n'
					<< "Raw String: " << rawValueString << '\n';
				std::cout << "Offset: " << offset << " | End Position: " << endPosition << '\n';
#endif
				break; 
			}
		}
#ifdef DEBUG
		std::cout << startPosition << ' ' << endPosition << '\n';
#endif
//		if (foundComma) {
//			std::size_t rawValuePosition = this->m_jsonData.find(",", endPosition + 1);
//			std::string rawValueString = this->m_jsonData.substr(endPosition + 1, rawValuePosition - endPosition + 1);
//#ifdef DEBUG
//			std::cout << "Raw string: " << ' ' << rawValueString << '\n';
//#endif
//			endPosition += rawValueString.size() - 2;
//		}
//		if (hasMoreValues) {
//			if (foundComma) endPosition++;
//			else endPosition -= 2; // BUG: If there is no white space after "
//		}
		ret = this->m_jsonData.substr(startPosition + 2, (endPosition) - (startPosition + 2));
	}
	else {
		ret = "";
	}
	return ret;
}