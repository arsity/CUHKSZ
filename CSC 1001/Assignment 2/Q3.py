def userInput():
    while True:
        cardNumber = input('Please input a card number: ')
        if cardNumber.isdecimal() and 13 <= len(cardNumber) <= 16:
            return cardNumber[::-1]
        print('Please input a positive integer having digits from 13 to 16!')


def sumOfOddPlace(cardNumber):
    sum = 0
    for position in range(0, len(cardNumber), 2):
        sum = sum+int(cardNumber[position])
    return sum


def getDigit(initialDigit):
    initialDigit = int(initialDigit)
    if initialDigit < 5:
        return 2*initialDigit
    return 2*initialDigit % 10+1


def sumOfDoubleEvenPlace(cardNumber):
    sum = 0
    for position in range(1, len(cardNumber), 2):
        sum = sum+getDigit(cardNumber[position])
    return sum


def isValid(cardNumber):
    if (sumOfDoubleEvenPlace(cardNumber)+sumOfOddPlace(cardNumber)) % 10 == 0:
        return True
    return False


def main():
    cardNumber = userInput()
    if isValid(cardNumber):
        print(cardNumber[::-1], 'is valid.')
    else:
        print(cardNumber[::-1], 'is invalid.')


main()
