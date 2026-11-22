import numpy as np
import matplotlib.pyplot as plt
import sys

if (len(sys.argv)<2):
    print("Provide a filename")
    quit(-1)

# Load TSV file
data = np.loadtxt(sys.argv[1])

# Extract columns
timestamp_ms = data[:, 0]  # Column 1: timestamp in ms
gyro_z = data[:, 6]       # Column 7: gyro Z

# Convert timestamps from ms to seconds
time = (timestamp_ms - timestamp_ms[0]) / 1000.0

# If gyro readings are in degrees/s, convert to rad/s:
# gyro_z = np.deg2rad(gyro_z)

# Integrate using trapezoidal rule
dt = np.diff(time)

angle = np.zeros(len(gyro_z))

angle[1:] = np.cumsum(
    0.5 * (gyro_z[:-1] + gyro_z[1:]) * dt
)

# Plot
plt.plot(time, np.rad2deg(angle))
plt.xlabel("Time (s)")
plt.ylabel("Angle (degrees)")
plt.title("Integrated Gyroscope Z-Axis Angle")
plt.grid(True)
plt.show()
