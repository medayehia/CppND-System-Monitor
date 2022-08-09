#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::stoi;
using std::string;
using std::to_string;
using std::vector;
// TODO: Return this process's ID
int Process::Pid() const { return p_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const
{
  float cpu_usage = 0.0;
  auto cpuPro = LinuxParser::CpuUtilization(Process::Pid());

  auto totaltime_ = stoi(cpuPro[13]) + stoi(cpuPro[14]);
  auto seconds_ =
      LinuxParser::UpTime() - (stoi(cpuPro[21]) / sysconf(_SC_CLK_TCK));
  cpu_usage = ((totaltime_ / sysconf(_SC_CLK_TCK)) / seconds_);

  return cpu_usage;
}

string Process::Command() { return LinuxParser::Command(Process::Pid()); }

string Process::Ram() { return LinuxParser::Ram(Process::Pid()); }

string Process::User() { return LinuxParser::User(Process::Pid()); }

long int Process::UpTime() { return LinuxParser::UpTime() -  LinuxParser::UpTime(Process::Pid()); }

bool Process::operator<(Process const &a) const
{
  return this->Process::CpuUtilization() > a.Process::CpuUtilization();
}