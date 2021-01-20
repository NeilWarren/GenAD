import numpy as np
import matplotlib.pyplot as plt

y_arr = []
x_arr = []
f = open("data_fwd.txt", "r")
i = 1
for x in f:
  y_arr.append(float(x))
  x_arr.append(i)
  i += 1

y_np_arr = np.array(y_arr)
x_np_arr = np.array(x_arr)

log_y_np_arr = np.log2(y_np_arr)

y_arr_r = []
x_arr_r = []
f_r = open("data_rev.txt", "r")
i = 1
for x in f_r:
  y_arr_r.append(float(x))
  x_arr_r.append(i)
  i += 1

y_np_arr_r = np.array(y_arr_r)
x_np_arr_r = np.array(x_arr_r)

log_y_np_arr_r = np.log2(y_np_arr_r)

y_arr_p = []
x_arr_p = []
f = open("data_parser.txt", "r")
i = 1
for x in f:
  y_arr_p.append(float(x))
  x_arr_p.append(i)
  i += 1

y_np_arr_p = np.array(y_arr_p)
x_np_arr_p = np.array(x_arr_p)

log_y_np_arr_p = np.log2(y_np_arr_p)

y_arr_p1 = []
x_arr_p1 = []
f = open("data_parser_eval.txt", "r")
i = 1
for x in f:
  y_arr_p1.append(float(x))
  x_arr_p1.append(i)
  i += 1

y_np_arr_p1 = np.array(y_arr_p1)
x_np_arr_p1 = np.array(x_arr_p1)

log_y_np_arr_p1 = np.log2(y_np_arr_p1)



plt.title('Input Size vs. Log_2 of Results')
plt.xlabel('Log Function Input Size')
plt.ylabel('Log_2 of Timed Results (microseconds)')
plt.plot(x_np_arr, log_y_np_arr, 'b')
plt.plot(x_np_arr_r, log_y_np_arr_r, 'r')
plt.plot(x_np_arr_p, log_y_np_arr_p, 'g')
plt.plot(x_np_arr_p1, log_y_np_arr_p1, '#800080')


plt.show()

f.close();
f_r.close();


