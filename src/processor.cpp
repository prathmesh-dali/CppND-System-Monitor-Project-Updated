#include "processor.h"

#include "linux_parser.h"

float Processor::Utilization() {
  long currentActive, currentIdle, currentTotal, prevActive, prevTotal,
      deltaTotal, deltaActive;

  currentActive = LinuxParser::ActiveJiffies();
  currentIdle = LinuxParser::IdleJiffies();
  currentTotal = LinuxParser::Jiffies();
  prevActive = PreviousActive();
  prevTotal = PreviousTotal();

  PreviousActive(currentActive);
  PreviousIdle(currentIdle);
  PreviousTotal(currentTotal);

  deltaActive = currentActive - prevActive;
  deltaTotal = currentTotal - prevTotal;

  return float(deltaActive) / float(deltaTotal);
}

long Processor::PreviousActive() const { return previousActive_; }

long Processor::PreviousIdle() const { return previousIdle_; }

long Processor::PreviousTotal() const { return previousTotal_; }

void Processor::PreviousActive(long value) { previousActive_ = value; }

void Processor::PreviousIdle(long value) { previousIdle_ = value; }

void Processor::PreviousTotal(long value) { previousTotal_ = value; }