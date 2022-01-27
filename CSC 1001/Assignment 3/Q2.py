from functools import reduce


class Polynomial:
    def __init__(self, polynomial='6*x^5-3.5*x+3.64-3.5*x^2+2.5*x^4'):
        if polynomial == '':
            polynomial = '6*x^5-3.5*x+3.64-3.5*x^2+2.5*x^4'
        if not polynomial.startswith('-'):
            polynomial = '+'+polynomial
        self.OriginalPolynomial = polynomial
        self.PolynomialList = []

    def __Cut(self):
        startPoint = 0
        index = 1
        while startPoint < len(self.OriginalPolynomial):

            if index > len(self.OriginalPolynomial)-1:
                cut = self.OriginalPolynomial[startPoint:index]
                self.PolynomialList.append(cut)
                break

            elif self.OriginalPolynomial[index] in ['+', '-']:
                cut = self.OriginalPolynomial[startPoint:index]
                self.PolynomialList.append(cut)
                startPoint = index
            index += 1

    def __Derivative(self):
        self.DerivativeList = list(map(derivative, self.PolynomialList))
        self.answer = str(reduce(lambda x, y: x+y, self.DerivativeList))

    def __str__(self) -> str:
        self.__Cut()
        self.__Derivative()
        return self.answer if self.answer[0] == '-' else self.answer[1:]


def is_number(a):
    try:
        a = float(a)
        return True
    except:
        return False


def derivative(item):
    multiple_Index = item.find('*')
    power_Index = item.find('^')

    if is_number(item):
        return ''

    if power_Index == -1:
        if multiple_Index == -1:
            return '+1'

        else:
            return item[0:multiple_Index]

    elif eval(item[power_Index+1:]) == 2:
        if multiple_Index == -1:
            return item[0]+'2'+item[1]

        else:
            coefficient = 2*eval(item[1:multiple_Index])
            if type(coefficient) == float and coefficient.is_integer():
                coefficient = int(coefficient)
            return item[0]+str(coefficient)+item[multiple_Index:multiple_Index+2]

    else:
        if multiple_Index == -1:
            return item[0]+item[power_Index+1:]+'*'+item[1]+'^'+str(eval(item[power_Index+1:])-1)

        else:
            coefficient = eval(item[power_Index+1:]) * \
                eval(item[1:multiple_Index])
            if type(coefficient) == float and coefficient.is_integer():
                coefficient = int(coefficient)
            return item[0]+str(coefficient)+'*'+item[multiple_Index+1]+'^'+str(eval(item[power_Index+1:])-1)


def main():
    a = Polynomial(input(
        'Please enter a polynomial in standard algebraic notation.\n(default as 6*x^5-3.5*x+3.64-3.5*x^2+2.5*x^4)\n>>'))
    print(a)


main()
