import numpy as np
import matplotlib.pyplot as plt 

x_data = []
y_data = []

n=0
h=0.0

x=[]
y_origin_plot = []
y_lagrange_plot=[]
y_newton_plot=[]

newton_parameter = np.zeros((1,1))


def init():
    global x_data,y_data,n,h,x,newton_parameter
    x_data = list(map(float,input("Please enter Xs:").strip().split(',')))
    y_data = list(map(float,input("Please enter Ys:").strip().split(',')))
    n = len(x_data)
    h = x_data[1]-x_data[0]
    x = np.linspace(x_data[0],x_data[n-1],10000)
    newton_parameter=np.zeros((n,n))
    for j in range(n):
        newton_parameter[0][j]=y_data[j]
    for i in range(n):
        for j in range(n):
            if i>0 and j>=i:
                newton_parameter[i][j]=newton_parameter[i-1][j]-newton_parameter[i-1][j-1]
    print(newton_parameter)


def Origin():
    for cur_x in x:
        y_origin_plot.append(1/(cur_x*cur_x+1))

def Lagrange():
    for cur_x in x:
        ans = 0.0
        for i in range(n):
            s = y_data[i]
            for j in range(n):
                if j != i:
                    s *= (cur_x - x_data[j])/(x_data[i]-x_data[j])
            ans += s
        y_lagrange_plot.append(ans)

def Newton():
    for cur_x in x:
        t = (cur_x-x_data[0])/h
        ans = y_data[0]
        for i in range(n):
            if i>0:
                ans_k = newton_parameter[i][i]
                for j in range(n):
                    if j>0 and j<=i:
                        ans_k /= j
                        ans_k *=(t-j+1)
                ans+=ans_k
        y_newton_plot.append(ans)


def Drawer():
    plt.title("Interpolation")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.plot(x,y_origin_plot,'r',label = "Origin")
    plt.plot(x,y_lagrange_plot,'g',label = "Lagrange")
    plt.plot(x,y_newton_plot,'b',label = "Newton")
    plt.legend()
    plt.show()

def main():
    init()
    Origin()
    Lagrange()
    Newton()
    Drawer()

if __name__ == '__main__':
    main()
