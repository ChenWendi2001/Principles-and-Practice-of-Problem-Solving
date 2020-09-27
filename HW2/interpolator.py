import numpy
import matplotlib

x_data = []
y_data = []


def Lagrange_get_interp_x(interp_x):
    n = len(x_data)
    ans = 0
    for i in range(n):
        s = y_data[i]
        for j in range(n):
            if j != i:
                s *= (interp_x - x_data[j])/(x_data[i]-x_data[j])
        ans += s
    return ans


## get original number
length = int(input("Input the number of points:"))
for i in range(length):
    x_data.append(float(input()))
for j in range(length):
    y_data.append(float(input()))


interp_x = float(input("Input the interp_x:"))
print(Lagrange_get_interp_x(interp_x))
