#pragma once

#include "Database.h"
#include <vector>
#include <string>
#include <utility>
#include <boost/algorithm/string.hpp>

class Registration 
{
public:
    Registration();
    std::pair<std::string, bool> handleRegistration(const char[]);
    
private:
    Database database;
};
