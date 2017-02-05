#pragma once

#include "Database.h"
#include <boost/algorithm/string.hpp>
#include <string>
#include <utility>
#include <vector>

class Registration
{
public:
	std::pair<std::string, bool> handleRegistration(const char[], Database&);
};
