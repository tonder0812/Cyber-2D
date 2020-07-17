#include <pch.h>
#include "log.h"
int main() {
	Cyber::Log::Init();
	CB_CORE_INFO("STARTING THE ENGINE");
	std::cin.get();
}