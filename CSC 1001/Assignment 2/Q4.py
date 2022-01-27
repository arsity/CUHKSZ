import string


def input_Test():
    while True:
        word = input('Please input a word: ')
        for letter in word:
            if letter not in string.ascii_letters:
                print('Invalid input!')
                break
        else:
            return word


def transform(word1, word2):
    wordList1 = list(word1.lower())
    wordList2 = list(word2.lower())
    wordList1.sort()
    wordList2.sort()
    return wordList1, wordList2


def result(wordList1, wordList2, word1, word2):
    if wordList1 == wordList2:
        print(word1, 'and', word2, 'is an anagram.')
    else:
        print(word1, 'and', word2, 'is not an anagram.')


def main():
    word1 = input_Test()
    word2 = input_Test()
    a, b = transform(word1, word2)
    result(a, b, word1, word2)


main()
