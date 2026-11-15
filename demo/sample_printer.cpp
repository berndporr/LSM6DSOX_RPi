#include "../LSM6DSOX.h"
#include <cstdio>

std::string get_elapsed_ms ();

const std::chrono::time_point<std::chrono::steady_clock> program_start
    = std::chrono::steady_clock::now ();

class Callback
{
  public:
    virtual void hasSample (const LSM6DSOXSample &s)
    {
        printf ("%ld %f %f %f %f %f %f\n", get_elapsed_ms (), s.ax, s.ay, s.az,
                s.gx, s.gy, s.gz);
    }

  private:
    long get_elapsed_ms () const
    {
        auto now = std::chrono::steady_clock::now ();
        auto elapsed = now - program_start;
        return std::chrono::duration_cast<std::chrono::milliseconds> (elapsed)
            .count ();
    }
};

int main (int, char **)
{
    Callback callback;
    LSM6DSOX lsm6dS0x;
    lsm6dS0x.registerCallback (
        [&] (const LSM6DSOXSample &s) { callback.hasSample (s); });
    LSMSettings lsmsettings;
    lsmsettings.gyroCalibration = true;
    lsmsettings.samplingRate = LSMSettings::ODR_12_5;
    try
    {
        lsm6dS0x.start (lsmsettings);
    } catch (const char *msg) {
        fprintf(stderr,"Error: %s\n",msg);
        return -1;
    }
    fprintf (stderr, "Sampling rate: %f\n", lsm6dS0x.getODR ());
    fprintf (stderr, "Press Enter to stop\n");
    getchar ();
    lsm6dS0x.stop ();
    return 0;
}
