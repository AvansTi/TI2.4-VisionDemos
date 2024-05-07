#pragma once

#include <string>
#include <list>

class MRU
{
public:
	std::list<std::string> files;

	MRU();
	void open(const std::string fileName);
};

