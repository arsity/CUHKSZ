from random import random
from functools import reduce


class Ecosystem:
    def __init__(self, LENGTH, N_FISH, N_BEAR) -> None:
        self.river = list('N'*LENGTH)
        self.LENGTH = LENGTH
        count1 = 0
        while count1 < N_FISH:
            index = int(random()*LENGTH)
            if self.river[index] == 'N':
                self.river[index] = 'F'
                count1 += 1
        count2 = 0
        while count2 < N_BEAR:
            index = int(random()*LENGTH)
            if self.river[index] == 'N':
                self.river[index] = 'B'
                count2 += 1
        print(str(reduce(lambda x, y: x+y, self.river)))

    def __exchange(self, index, towards='R'):
        if towards == 'R':
            x = 1
        else:
            x = -1
        if self.river[index] != self.river[index+x]:
            if self.river[index+x] == 'N':
                self.river[index+x] = self.river[index]
                self.river[index] = 'N'
                if x == 1:
                    self.count += 1
            else:
                flag = False
                if self.river[index] == 'B' and x == 1:
                    flag = True
                self.river[index+x] = 'B'
                self.river[index] = 'N'
                if flag:
                    self.count += 1
        else:
            if self.river[index] == 'F':
                self.Accumulate.append('F')
            elif self.river[index] == 'B':
                self.Accumulate.append('B')

    def __move(self, index):
        if index == 0 and random() > 0.5:
            self.__exchange(index, 'R')
        elif index == len(self.river)-1 and random() > 0.5:
            self.__exchange(index, 'L')
        elif index > 0 and index < len(self.river)-1:
            move = int(random()*3)
            if move == 1:
                self.__exchange(index, 'L')
            elif move == 2:
                self.__exchange(index, 'R')

    def simulate(self, times=1):
        for i in range(times):
            self.count = 0
            self.Accumulate = []
            while self.count <= len(self.river)-1:
                if self.river[self.count] != 'N':
                    self.__move(self.count)
                self.count += 1
            None_List = []
            index = 0
            while index <= len(self.river)-1:
                if self.river[index] == 'N':
                    None_List.append(index)
                index += 1
            if None_List != []:
                if len(None_List) <= len(self.Accumulate):
                    for k in range(len(None_List)):
                        index = None_List[int(random()*len(None_List))]
                        self.river[index] = self.Accumulate[k]
                        None_List.remove(index)
                else:
                    for k in self.Accumulate:
                        index = None_List[int(random()*len(None_List))]
                        self.river[index] = k
                        None_List.remove(index)
            self.answer = str(reduce(lambda x, y: x+y, self.river))
            print(self.answer)
            if self.answer in ['B'*self.LENGTH, 'F'*self.LENGTH]:
                print('Simulation End!')
                break


def main():
    a = Ecosystem(10, 4, 2)
    while True:
        a.simulate(int(input('How many times?\n>>')))
        if a.answer in ['B'*a.LENGTH, 'F'*a.LENGTH]:
            break


main()
