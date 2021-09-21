#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // We need to operate through analyzing tokens so this creates each line into a key-value line consisting of two tokens. 
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream>>key>>value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
// https://www.guru99.com/cpp-file-read-write-open.html
float LinuxParser::MemoryUtilization() 
{
  string line;
  string key, value, size;
  float total, free;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open())
  {
    while (1)
    {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value >> size;
      if (key == "MemTotal:")
      {
        total = stof(value);
      }
      if (key == "MemFree:")
      {
        free = stof(value);
        break;
      }

    }


  }
  
  return total - free;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  string line;
  string key;
  int value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while (1)
    {
      std::getline(stream, line, '\n');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes")
      {
        return value;
      }
    }
  }
  // http://cplusplus.com/forum/beginner/189237/ 
  // https://github.com/microsoft/vscode/issues/38133
  // https://stackoverflow.com/questions/13510453/c-reading-an-multiline-file-with-lines-with-arbitary-lengths-and-format-withou
  // https://www.softwaretestinghelp.com/stringstream-class-in-cpp/
  // https://en.cppreference.com/w/cpp/io/manip/setw
  // https://stackoverflow.com/questions/30567930/reading-a-file-with-variable-line-lengths-c
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  string line;
  long system_uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open())
  {
    std::getline(stream, line, '\n');
    std::istringstream linestream(line);
    linestream >> system_uptime;
    return system_uptime;
  } 
}

// TODO: Read and return the number of jiffies for the system
//https://superuser.com/questions/88820/how-to-obtain-the-current-number-of-jiffies-since-reboot-in-linux
//https://www.anshulpatel.in/post/linux_cpu_percentage/
//https://www.idnt.net/en-US/kb/941772
long LinuxParser::Jiffies() 
{ 
  string line;
  string cpu;
  int user_value, nice_value, system_value;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open())
  {
    std::getline(stream, line, '\n');
    std::istringstream linestream(line);
    linestream >> cpu >> user_value >> nice_value >> system_value;
    return system_value; 
  }
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  string line;
  string cpu;
  int user_value, nice_value, system_value, idle_value;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open())
  {
    std::getline(stream, line, '\n');
    std::istringstream linestream(line);
    linestream >> cpu >> user_value >> nice_value >> system_value >> idle_value;
    return idle_value; 
  } 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  string line;
  string key;
  int value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while (1)
    {
      std::getline(stream, line, '\n');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running")
      {
        return value;
      }
    }
  }
}

// TODO: Read and return CPU utilization
// Make this a child of the existing Bonus Question or find a way to invoke upon that class....maybe use of "Friend"?
vector<string> LinuxParser::CpuUtilization() 
{ 
  return {};
}

int main()
{
  vector<int> test = LinuxParser::Pids();

}
