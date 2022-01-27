def initial_Input():
    while True:
        n = input("Please enter a nonnegative number: ")
        try:
            n = float(n)
            if n < 0:
                print('Invalid input!')
                continue
            return n
        except:
            print('Invalid input!')


def approximate(n):
    lastGuess = 1
    while True:
        nextGuess = (lastGuess+(n/lastGuess))/2
        if abs(nextGuess-lastGuess) < 0.0001:
            return nextGuess
        lastGuess = nextGuess


def main():
    print('%.3f' % approximate(initial_Input()))


main()
