class flower:
    def __init__(self) -> None:
        self.Name = 'rose'
        self.Number = 3
        self.Price = 10.00

    def name(self):
        return self.Name

    def setName(self):
        while True:
            name = input('Please enter the name of flower: ')
            if len(name) != 0:
                self.Name = name
                break

    def number(self):
        return self.Number

    def setNumber(self):
        while True:
            number = input('Please enter the number of flower: ')
            if number.isnumeric() and int(number) >= 0:
                self.Number = int(number)
                break
            print('Please enter a non-negative integer.')

    def price(self):
        return self.Price

    def setPrice(self):
        while True:
            price = input('Please enter the price of flower: ')
            try:
                price = float(price)
                if price <= 0:
                    print('Please enter a postive float.')
                    continue
                self.Price = price
                break
            except:
                print('Please enter a positive float.')


def main():
    a = flower()
    print(a.name(), a.number(), a.price())
    a.setName()
    a.setNumber()
    a.setPrice()
    print(a.name(), a.number(), a.price())


main()
