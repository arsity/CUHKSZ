# input
N = input("Enter a POSITIVE INTEGER N: ")

# conditions
prime_Number_List = [2]

# number test


def number_test(n):
    result = None
    try:
        n = int(N)
        if n <= 2:
            print("Please enter an INTEGER which is LARGER than 2!")
            result = False
        elif n == 3:
            print("The prime number smaller than 3 include: \n2")
            result = False
        else:
            result = True
    except:
        print("Please enter a POSITIVE INTEGER!")
        result = False
    return result

# make a prime number list for numbers smaller than n


def find_prime_number_list(n):
    global prime_Number_List
    for number in range(3, n):
        for base in prime_Number_List:  # need to improve
            if number % base == 0:
                break
            elif base >= int(number**0.5+1):
                prime_Number_List.append(number)
                break
    return prime_Number_List

# print


def print_with_8():
    print("The prime numbers smaller than", int(N), "include: ")
    global prime_Number_List
    count = 1
    for prime_Number in prime_Number_List:
        if count % 8 == 0:
            print(prime_Number, end="\n")
            count += 1
        else:
            print(prime_Number, end="\t")
            count += 1


if number_test(N) == True:
    prime_Number_List = find_prime_number_list(int(N))
    print_with_8()
