#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <unistd.h>

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  unsigned long long int idlealltime = 0;
  unsigned long long int systemalltime = 0;
  unsigned long long int virtalltime = 0;
  unsigned long long int totaltime = 0;

  unsigned long long int pre_idealltime = 0;
  unsigned long long int pre_totaltime = 0;

  float cu_totaltime = 0;
  float cu_idealltime = 0;
};

#endif