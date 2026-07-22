#include "LSM6DSOX.h"
#include <cstdio>

class Callback {
  public:
  virtual void hasSample(const LSM6DSOXSample &s) {
    printf("%f %f %f %f %f %f\n",s.ax,s.ay,s.az,s.gx,s.gy,s.gz);
  }
};
 
int main(int, char **) {
  Callback callback;
  LSM6DSOX lsm6dS0x;
  lsm6dS0x.registerCallback([&](const LSM6DSOXSample &s){callback.hasSample(s);});
  lsm6dS0x.start();
  getchar();
  lsm6dS0x.stop();
  return 0;
}
