#include "LSM6DSOX.h"

class Callback : public LSM6DSOX::LSM6DSOXCallback {
  virtual void hasSample(const LSM6DSOXSample &s) {
    printf("%f %f %f %f %f %f\n",s.ax,s.ay,s.az,s.gx,s.gy,s.gz);
  }
};
 
int main(int, char **) {
  Callback callback;
  LSM6DSOX lsm6dS0x;
  lsm6dS0x.registerCallback(&callback);
  lsm6dS0x.start();
  getchar();
  lsm6dS0x.stop();
  return 0;
}
