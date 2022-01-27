import math  # import math to support sine
import numpy as np  # import numpy to get more precise


# calculate double factorial
def doubleFactorial(n: np.longdouble):
    if n == 1:
        return 1
    else:
        return n*doubleFactorial(n-np.longdouble(2))


# the ith term of infinite series
def term(i: np.longdouble, rad=np.longdouble(3)):
    return (np.longdouble(doubleFactorial(2*i-1))/(np.longdouble(2)**i*np.longdouble(math.factorial(i))))**np.longdouble(2)*np.longdouble(math.sin(rad/np.longdouble(2))**(np.longdouble(2)*i))


def main():
    count = 2
    error = np.longdouble(1.0)
    while (np.abs(error) > 0.00001):
        error = np.longdouble(np.longdouble(term(count)) -
                              np.longdouble(term(count-1)))
        count += 1
    print(count)


main()
