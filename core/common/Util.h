#pragma once

#include <string>
#include <initializer_list>

std::string ssprintf(const char* format, ...);
bool sreplace(std::string& str, const std::string& from, const std::string& to);
bool sreplace(std::string& str, const std::initializer_list<std::string>& from, const std::string& to);
