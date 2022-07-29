#include "format.h"

#include <string>

using std::string;
using std::to_string;

string Format::ElapsedTime(long seconds) {
  long min = seconds / 60;
  long remSec = seconds % 60;
  long hrs = min / 60;
  long remMin = min % 60;
  return to_string(hrs) + ':' + to_string(remMin) + ':' + to_string(remSec);
}