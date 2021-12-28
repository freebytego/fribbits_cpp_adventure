#include <iostream>
#include <string>

int main()
{
	std::string my_str{ "understand" };
	std::string temp_str{ "" };
	for (int i{ 0 }; i < my_str.size(); ++i) {
		temp_str += my_str[i];
		std::cout << temp_str << '\n';
	}
	for (int i{ 0 }; i < my_str.size(); ++i) {
		temp_str.pop_back();
		std::cout << temp_str << '\n';
	}
}
