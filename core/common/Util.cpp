#include <common/Util.h>
#include <stdarg.h>

using namespace std;

string ssprintf(const char* format, ...) {
  const unsigned int bufsize = 4096;
  thread_local char buffer[bufsize];
  va_list args;
  va_start(args, format);
  vsnprintf (buffer, bufsize, format, args);
  va_end (args);
  return string(buffer);
}

bool sreplace(std::string& str, const std::string& from, const std::string& to) {
  size_t start_pos = str.find(from);
  if(start_pos == std::string::npos)
    return false;
  str.replace(start_pos, from.length(), to);
  return true;
}

bool sreplace(std::string& str, const std::initializer_list<std::string>& from, const std::string& to) {
  for(const auto& s : from)
    sreplace(str, s, to);
}
