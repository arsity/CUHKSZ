def initialize():
    lockerList = []
    for counts in range(0, 100):
        lockerList.append(False)
    return lockerList


def operation(lockerList):
    for student in range(1, 101):
        for lockerNumber in range(student-1, 100, student):
            lockerList[lockerNumber] = not lockerList[lockerNumber]
    return lockerList


def display(lockerList):
    for index in range(0, 100):
        if lockerList[index]:
            lockerList[index] = 'Open'
        else:
            lockerList[index] = 'Closed'
    count = 1
    for locker in lockerList:
        if count % 10 == 0:
            print('%10s' % locker, end='\n')
        else:
            print('%10s' % locker, end=' ')
        count += 1


def main():
    display(operation(initialize()))


main()
