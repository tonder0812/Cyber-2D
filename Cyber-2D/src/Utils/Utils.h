#pragma once
#include <string>
namespace Cyber {
	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter, bool changeLocation = false);
		static std::string SaveFile(const char* filter, bool changeLocation = false);
	};
}