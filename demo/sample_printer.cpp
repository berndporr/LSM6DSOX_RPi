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
        // 1. Get the current steady time point
        auto now = std::chrono::steady_clock::now ();

        // 2. Calculate the difference since the program started
        auto elapsed = now - program_start;

        // 3. Cast the duration to milliseconds
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
    lsm6dS0x.start ();
    getchar ();
    lsm6dS0x.stop ();
    return 0;
}
