#include "Registration.h"
#include <iostream>

Registration::Registration() : database("database.db")
{
}

std::pair<std::string, bool> Registration::handleRegistration(const char registrationString[])
{
    std::vector<std::string> registrationParts;
    boost::split(registrationParts, registrationString, boost::is_any_of(" "));
    if (registrationParts.size() == 0) 
    {
        // expected format is "register <username> <password>", if the registrationString doesn't match the format, abort!
        return {"0", false}; 
    }
    
    auto users = database.execute("SELECT username FROM users WHERE username = ?", {registrationParts.at(1)});
    if (users.size() != 0)
    {
        return {registrationParts.at(1), false}; // username already exists, abort!
    }
    
    //std::cout << "Your username is: " << registrationParts.at(1) << " and password is: " << registrationParts.at(2) << "\n";
    
    database.execute("INSERT INTO users(username, password) VALUES(?, ?)", {registrationParts.at(1), registrationParts.at(2)});
    
    users = database.execute("SELECT username FROM users WHERE username = ?", {registrationParts.at(1)});
    if (users.size() != 0)
    {
        return {registrationParts.at(1), true}; // was the user successfully added to the database?
    }
    
    return {registrationParts.at(1), false};
}
