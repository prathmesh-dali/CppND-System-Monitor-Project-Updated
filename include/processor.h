#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();
  long PreviousActive() const;
  long PreviousTotal() const;
  long PreviousIdle() const;
  void PreviousActive(long value);
  void PreviousTotal(long value);
  void PreviousIdle(long value);

 private:
  long previousActive_{0};
  long previousTotal_{0};
  long previousIdle_{0};
};

#endif