class Node:
    def __init__(self, element: str, pointer):
        self.element = element
        self.pointer = pointer


class SinglyLinkedList:
    def __init__(self):
        self.head = Node('PKX', None)
        self.head = Node('PEK', self.head)

    def insert(self, data: str):
        self.head = Node(data, self.head)

    def recursive_count(self, node: Node):
        if node.pointer == None:
            return 1
        else:
            return 1+self.recursive_count(node.pointer)


a = SinglyLinkedList()
a.insert('CAN')
a.insert('SHA')
a.insert('SZX')
print(a.recursive_count(a.head))
