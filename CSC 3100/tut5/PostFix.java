import java.util.*;

public class PostFix {
    public static String post2in(String postfix) {
        String[] elements = postfix.split(" ");
        System.out.println(Arrays.toString(elements));
        Stack<String> s = new Stack<String>();
        for (String e : elements) {
            if ()){

            }
        }
        return // To Do
    }

    public static float evalPost(String postfix) {
        String[] elements = postfix.split(" ");
        System.out.println(Arrays.toString(elements));
        Stack<Float> s = new Stack<Float>();
        for (String e : elements) {
            // To Do
        }
        return // To Do
    }

    public static void main(String[] args) {
        System.out.println(post2in("3 2 + 4 /"));
        System.out.println(evalPost("3 2 + 4 /"));
    }
}