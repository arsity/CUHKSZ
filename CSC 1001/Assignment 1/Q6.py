# dependencies
from math import sin
from math import cos
from math import tan

# collect data
f = input("Please choose a function from sin, cos and tan: ")
n = input("Please enter a POSITIVE INTEGER n: ")
a = input("Please enter the Smaller endpoint a: ")
b = input("Please enter the Larger endpoint b: ")

# input test


def input_test(f, n, a, b):
    result = None
    if f in ['sin', 'cos', 'tan']:
        try:
            n = int(n)
            if n <= 0:
                result = False
                print("Please enter a POSITIVE INTEGER!")
            else:
                try:
                    a = float(a)
                    b = float(b)
                    result = True
                except:
                    result = False
                    print("INVALID endpoint!")
        except:
            result = False
            print("Please enter a POSITIVE INTEGER for n!")
    else:
        result = False
        print("INVALID Function!")
    return result


if input_test(f, n, a, b) == True:
    sum = 0
    n = int(n)
    a = float(a)
    b = float(b)
    for i in range(1, n+1):
        sum += (((b-a)/n)*(eval(f)(a+(b-a)/n*(i-1/2))))
    print(sum)
