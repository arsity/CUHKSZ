import java.util.*;

class MyStack {
    Queue<Integer> queue1;
    Queue<Integer> queue2;

    public MyStack() {
        queue1 = new LinkedList<Integer>();
        queue2 = new LinkedList<Integer>();
    }

    public void push(int x) {
        queue2.add(x);
        while (!queue1.isEmpty()) {
            queue2.add(queue1.remove());
        }
        while (!queue2.isEmpty()) {
            queue1.add(queue2.remove());
        }
    }

    public int pop() {
        return queue1.remove();
    }

    public boolean empty() {
        return queue1.isEmpty();
    }
}

public class stackUsingQueue {
    public static void main(String[] args) {
        int n = 5;
        MyStack stack = new MyStack();
        for (int i = 0; i < n; i++) {
            stack.push(i);
        }
        while (!stack.empty()) {
            System.out.println(stack.pop());
        }
    }
}
