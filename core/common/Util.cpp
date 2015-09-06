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
