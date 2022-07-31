#include "format.h"
#include <iomanip>
#include <string>

using std::string;
using std::to_string;

string Format::ElapsedTime(long seconds) {
  int min = seconds / 60;
  int remSec = seconds % 60;
  int hrs = min / 60;
  int remMin = min % 60;
  return to_string(hrs) + ':' + to_string(remMin) + ':' + to_string(remSec);
}