import numpy as np
import matplotlib.pyplot as plt

y_arr = []
x_arr = []
f = open("data.txt", "r")
i = 5
for x in f:
  y_arr.append(float(x))
  x_arr.append(i)
  i += 1

y_np_arr = np.array(y_arr)
x_np_arr = np.array(x_arr)

log_y_np_arr = np.log2(y_np_arr)

plt.title('Input Size vs. Log_2 of Results')
plt.xlabel('Function Input Size')
plt.ylabel('Log_2 of Timed Results (microseconds)')
plt.plot(x_np_arr, log_y_np_arr)

plt.show()



