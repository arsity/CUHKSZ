# input
N = input("Enter a POSITIVE INTEGER N: ")

# input test
try:
    n = int(N)
    if n <= 0:
        print("Please enter a POSITIVE INTEGER!")
    else:

        # print
        print("m\tm+1\tm**(m+1)")
        for m in range(1, n+1):
            print(m, "\t", m+1, "\t", m**(m+1), sep="")
except:
    print("Please enter a POSITIVE INTEGER!")
