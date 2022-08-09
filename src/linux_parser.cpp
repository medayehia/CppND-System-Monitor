#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem()
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "PRETTY_NAME")
        {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel()
{
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids()
{
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr)
  {
    // Is this a directory?
    if (file->d_type == DT_DIR)
    {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit))
      {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization()
{
  string key, value;
  string line;
  float memTotal, memFree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "MemTotal:")
        {
          memTotal = std::stof(value);
        }
        if (key == "MemFree:")
        {
          memFree = std::stof(value);
        }
      }
    }
  }
  return (memTotal - memFree) / memTotal;
}

long LinuxParser::UpTime()
{
  string line;
  long upTime = 0;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    upTime = std::stof(line);
  }
  return upTime;
}

vector<string> LinuxParser::CpuUtilization()
{
  string line;
  string key;
  vector<string> cpuStates;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu")
      {
        std::istream_iterator<string> begin(linestream);
        std::istream_iterator<string> end;
        vector<string> cpuStates(begin, end);
        return cpuStates;
      }
    }
  }
  return cpuStates;
}

int LinuxParser::TotalProcesses()
{
  string line;
  string key, value;
  int num_processes = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "processes")
        {
          num_processes = std::stoi(value);
        }
      }
    }
  }
  return num_processes;
}

int LinuxParser::RunningProcesses()
{
  string line;
  string key, value;
  int run_processes = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "procs_running")
        {
          run_processes = std::stoi(value);
        }
      }
    }
  }
  return run_processes;
}

string LinuxParser::Command(int pid)
{
  string line;
  string pid_name = to_string(pid);
  std::ifstream stream(kProcDirectory + pid_name + kCmdlineFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
  }
  return line;
}

string LinuxParser::Ram(int pid)
{
  string line;
  string pid_name = to_string(pid);
  string key, value;
  int ram = 0;
  std::ifstream stream(kProcDirectory + pid_name + kStatusFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:")
      {
        ram = std::stoi(value);
        // return to_string(ram/1024);
      }
    }
  }
  return to_string(ram / 1024);
}

string LinuxParser::Uid(int pid)
{
  string line;
  string pid_name = to_string(pid);
  string key, value;
  std::ifstream stream(kProcDirectory + pid_name + kStatusFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:")
      {
        return value;
      }
    }
  }
  return "Uid not Found";
}

string LinuxParser::User(int pid)
{
  string uid = Uid(pid);
  string line;
  string key, value, value2;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> value2)
      {
        if (value2 == uid)
        {
          return key;
        }
      }
    }
  }
  return "userName not found";
}

long LinuxParser::UpTime(int pid)
{
  string pid_name = to_string(pid);
  string line;
  int t = 0;
  std::ifstream stream(kProcDirectory + pid_name + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);

    std::istringstream linestream(line);
    std::istream_iterator<string> begin(linestream);
    std::istream_iterator<string> end;
    vector<string> lineData(begin, end);
    t = std::stoi(lineData[21]);
    return t / sysconf(_SC_CLK_TCK);
  }
}

vector<std::string> LinuxParser::CpuUtilization(int pid)
{
  string pid_name = to_string(pid);
  string line;
  string key;
  std::ifstream stream(kProcDirectory + pid_name + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::istream_iterator<string> begin(linestream);
    std::istream_iterator<string> end;
    vector<string> cpuStates(begin, end);
    return cpuStates;
  }
  return {0};
}
