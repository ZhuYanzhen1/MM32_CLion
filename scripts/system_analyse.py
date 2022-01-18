from control import *
from control import matlab
from matplotlib import pyplot as plt

kp = 8.8371
kd = 0.4945
ki = 39.4847
sys1 = tf([400*kd, 400*kp, 400*ki], [1, 30, 200, 0, 0])


# Step Response
def step_resp():
    sout1, stime1 = matlab.step(sys1)
    plt.plot(stime1, sout1, 'b', linewidth=0.5)
    plt.xlabel("Time")
    plt.ylabel("Amplitude")
    plt.title("Step Resopnse", fontsize=12)
    plt.show()


# Impulse Response
def impulse_resp():
    iout1, itime1 = matlab.impulse(sys1)
    plt.plot(itime1, iout1, 'm', linewidth=0.8)
    plt.xlabel("Time")
    plt.ylabel("Amplitude")
    plt.title("Impulse Resopnse", fontsize=12)
    plt.show()


step_resp()
impulse_resp()
bode(sys1)
plt.show()
nyquist_plot(sys1)
plt.show()
rlocus(sys1)
plt.show()
