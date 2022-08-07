import java.util.*;

public class Josephus {
    public static void main(String[] args) {
        int n = 5;
        int k = 3;
        Queue<Integer> queue = new LinkedList<Integer>();

        for (int i = 1; i <= n; i++)
            queue.add(i);

        // TO DO
        while (queue.size() != 1) {
            for (int i = 0; i < k; i++) {
                queue.add(queue.remove());
            }
            queue.remove();
        }
    }
}
