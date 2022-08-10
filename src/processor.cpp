#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  auto cpuUnits = LinuxParser::CpuUtilization();
  std::vector<int> cpuArr;

  for (std::string s : cpuUnits) {
    cpuArr.push_back(std::stoi(s));
  }

  idlealltime = cpuArr[3] + cpuArr[4];
  systemalltime = cpuArr[2] + cpuArr[5] + cpuArr[6];
  virtalltime = cpuArr[8] + cpuArr[9];
  totaltime = cpuArr[0] + cpuArr[1] + systemalltime + idlealltime + cpuArr[7] +
              virtalltime;

  cu_idealltime = idlealltime - pre_idealltime;
  cu_totaltime = totaltime - pre_totaltime;

  pre_idealltime = idlealltime;
  pre_totaltime = totaltime;

  return (cu_totaltime - cu_idealltime) / cu_totaltime;
}
