def prime_Test(n):
    for num in range(2, int(n**0.5+1)):
        if n % num == 0:
            return False
    return True


def find(n):
    emirpList = []
    num = 11
    while len(emirpList) < n:
        reversenum = int(str(num)[::-1])
        if num == reversenum:
            num += 1
            continue
        if prime_Test(num) and prime_Test(reversenum):
            emirpList.append(num)
        num += 1
    emirpList.sort()
    return emirpList


def display(emirpList):
    count = 1
    for item in emirpList:
        if count % 10 == 0:
            print('%5d' % item, end='\n')
        else:
            print('%5d' % item, end=' ')
        count += 1


def main(n):
    display(find(n))


main(100)
