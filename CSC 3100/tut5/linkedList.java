class Node {
    public int value;
    public Node next;

    public Node(int data) {
        value = data;
        next = null;
    }

    public void display() {
        System.out.print("{" + value + "} ");
    }
}

public class linkedList {
    static Node head;

    public void insertFirst(int data) {
        Node newNode = new Node(data);
        newNode.next = head;
        head = newNode;
    }

    public void print() {
        Node current = head;
        while (current != null) {
            current.display();
            current = current.next;
        }
    }

    public void reversePrint(Node currentNode) {
        Node tail = currentNode;
        while (tail != head) {
            Node i = head;
            while (i.next != tail) {
                i = i.next;
            }
            tail.display();
            tail = i;
        }
        tail.display();
    }

    public void oddPrint(Node currentNode) {
        while ()
    }

    public static void main(String[] args) {
        linkedList myLinkedList = new linkedList();
        myLinkedList.insertFirst(5);
        myLinkedList.insertFirst(4);
        myLinkedList.insertFirst(3);
        myLinkedList.insertFirst(2);
        myLinkedList.insertFirst(1);
        System.out.print("Printing linked list (head --> tail): ");
        myLinkedList.print();
        System.out.print("Printing linked list (tail --> head): ");
        myLinkedList.reversePrint(head);
        System.out.println();
        // System.out.print("Printing linked list (print odd nodes forward and then backward): ");
        // myLinkedList.oddPrint(head);
    }
}
