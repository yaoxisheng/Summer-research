#include <iostream>
#include <sys/resource.h>
#include <sys/time.h>

class Timer{  
  struct rusage startu,endu;
  double duration;

public:
  void timer(){
    getrusage(RUSAGE_SELF, &startu);
  }
  void stopTime(){
    getrusage(RUSAGE_SELF, &endu);
    double start_sec = startu.ru_utime.tv_sec + startu.ru_utime.tv_usec/1000000.0;
    double end_sec = endu.ru_utime.tv_sec +endu.ru_utime.tv_usec/1000000.0;
    duration = end_sec - start_sec;
  }
  double getTime(){
    return duration;
  }
};
