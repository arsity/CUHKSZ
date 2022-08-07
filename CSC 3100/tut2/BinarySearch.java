public class BinarySearch {
     static int binarySearch(int data[], int target, int low, int high) {
        // TO DO
        return -1;
    }

    public static void main(String args[]) {
        int data[] = { 3, 4, 5, 6, 7, 8, 9 };
        int n = data.length;
        int target = 4;
        int position = binarySearch(data, target, 0, n - 1);
        if (position == -1) {
            System.out.println("Could not find this target in the given data!");
        } else {
            System.out.println("The position of this target is " + position + ".");
        }
    }
}