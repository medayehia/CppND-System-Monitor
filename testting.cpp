

#include <dirent.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};


float mem() {
  string key, value;
  string line;
  float memTotal, memFree, memAva, buffer, cached, swapCashed;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = std::stof(value);
        }
        if (key == "MemFree:") {
          memFree = std::stof(value);
        }
        if (key == "MemAvailable:") {
          memAva = std::stof(value);
        }
        if (key == "Buffers:") {
          buffer = std::stof(value);
        }
        if (key == "Cached:") {
          cached = std::stof(value);
        }
        if (key == "SwapCached:") {
          swapCashed = std::stof(value);
        }
      }
    }
  }
  std::cout << "MemUtilization: " << memTotal - memFree << std::endl;
  // std::cout << "MemFree: " << memFree << std::endl;
  // std::cout << "MemAvailable: " << memAva << std::endl;
  // std::cout << "Buffers: " << buffer << std::endl;
  // std::cout << "Cached: " << cached << std::endl;
  return 0;
}

long UpTime() {
  string line;
  long upTime = 0;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    upTime = std::stof(line);
    std::cout << "uptime: " << upTime << std::endl;
  }
  return upTime;
}

int TotalProcesses() {
  string line;
  string key, value;
  int num_processes = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          num_processes = std::stoi(value);
        }
      }
    }
  }
  return num_processes;
}

vector<string> CpuUtilization() {
  string line;
  string key;
  vector<string> cpuStates;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu") {
        std::istream_iterator<string> begin(linestream);
        std::istream_iterator<string> end;
        vector<string> cpuStates(begin, end);
        return cpuStates;
      }
    }
  }
  return cpuStates;
}

string Uid(int pid) {
  string line;
  string pid_name = to_string(pid);
  string key, value;
  std::ifstream stream(kProcDirectory + pid_name + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }
  }
  return "Uid not found";
}
string User(int pid) {
  string uid = Uid(pid);
  string line;
  string key, value, value2;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> value2) {
        if (value2 == uid) {
          return key;
        }
      }
    }
  }
  return "userName not found";
}

long upTime(int pid) {
  string pid_name = to_string(pid);
  string line;
  int t = 0;
  std::ifstream stream(kProcDirectory + pid_name + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);

    std::istringstream linestream(line);
    std::istream_iterator<string> begin(linestream);
    std::istream_iterator<string> end;
    vector<string> lineData(begin, end);
    std::cout << "upTime: " << lineData[21] << std::endl;
    t = std::stoi(lineData[21]);
  }
  return t / sysconf(_SC_CLK_TCK);
}

string ElapsedTime(long seconds) {
  long min = seconds / 60;
  long remSec = seconds % 60;
  long hrs = min / 60;
  long remMin = min % 60;
  return to_string(hrs) + ':' + to_string(remMin) + ':' + to_string(remSec);
}

std::vector<std::string> CpuUtilization(int pid) {
  string pid_name = to_string(pid);
  string line;
  string key;
  std::ifstream stream(kProcDirectory + pid_name + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::istream_iterator<string> begin(linestream);
    std::istream_iterator<string> end;
    vector<string> cpuStates(begin, end);
    return cpuStates;
  }
  return {0};
}


int main() {
  auto cpu = CpuUtilization(3515);
  for( std::string c : cpu)
  {
    std::cout << "cpu states: " << c << std::endl;
  }
}
