#include "format.h"
#include <iomanip>
#include <string>

using std::string;
using std::to_string;

string Format::ElapsedTime(long seconds)
{
  int min = seconds / 60;
  int remSec = seconds % 60;
  int hrs = min / 60;
  int remMin = min % 60;
  string hrrs = to_string(hrs);
  string mm = to_string(remMin);
  string ss = to_string(remSec);
  return hrrs.insert(0, 2 - hrrs.length(), '0') + ':' + mm.insert(0, 2 - mm.length(), '0') + ':' + ss.insert(0, 2 - ss.length(), '0');
}