class Node:
    def __init__(self,e,node):
        self.element=e
        self.pointer=node

class LinkedQueue:
    
    def __init__(self):
        self.head=None
        self.tail=None
        self.size=0

    def __len__(self):
        return self.size

    def is_empty(self):
        return self.size==0

    def first(self):
        if self.is_empty():
            print('Queue is empty.')
        else:
            return self.head.element

    def end(self):
        if self.is_empty():
            print('Queue is empty.')
        else:
            node=self.head
            while node != None:
                end_point=node.element
                node=node.pointer
            return end_point

    def dequeue(self):
        if self.is_empty():
            print('Queue is empty.')
        else:
            answer=self.head.element
            self.head=self.head.pointer
            self.size-=1
            if self.is_empty():
                self.tail=None
            return answer

    def enqueue(self,e):
        newest=Node(e,None)
        if self.is_empty():
            self.head=newest
        else:
            self.tail.pointer=newest
        self.tail=newest
        self.size+=1

    def __str__(self):
        queue=[]
        node=self.head
        while node != None:
            queue.append(str(node.element))
            node=node.pointer
        return str(queue)


def partition(Lq):
    global result
    big = LinkedQueue()
    small = LinkedQueue()
    current = Lq.head
    goal = Lq.tail    
    
    while goal != current:
        if current.element > goal.element:
            big.enqueue(Lq.dequeue())
        elif current.element <= goal.element:
            small.enqueue(Lq.dequeue())
        current = Lq.head    
    
    if  not small.is_empty():
        if small.size == 1:
            result.enqueue(small.dequeue())
        else:
            partition(small)    
    
    result.enqueue(Lq.dequeue())    
    
    if  not big.is_empty():
        if big.size == 1:
            result.enqueue(big.dequeue())
        else:
            partition(big)


def quickSort(node):
    global init, result
    result = LinkedQueue()
    current = node
    init = LinkedQueue()
    while current != None:
        init.enqueue(current.element)
        current = current.pointer
    partition(init)
    return result.head


def main():        
    node1 = Node(21,None)
    node2 = Node(223,node1)
    node3 = Node(4532,node2)
    node4 = Node(23,node3)
    node5 = Node(1,node4)
    node6 = Node(789,node5)
    node7 = Node(90,node6)
    node8 = Node(38,node7)

    a = quickSort(node8)
    print(result.__str__())
    

main()