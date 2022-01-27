# input
m = float(input("Enter the number m: "))

# the "while" part
if m < 0:
    n = int(m)+1
else:
    n = 0
    while n**2 <= m:
        n += 1

# output
print(n)
