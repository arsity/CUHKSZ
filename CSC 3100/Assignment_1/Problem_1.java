public class Problem_1 {

    public static void main(String[] args) {
        int[] A = { 3, 41, 52, 26, 38, 57, 9, 49 }; // initialize test array
        A = mergeSort(A); // merge sort
        for (int i = 0; i < A.length; i++) { // print
            System.out.println(A[i]);
        }
    }

    private static int[] mergeSort(int[] A) {
        int length = A.length;

        // basic situation
        if (length == 1) {
            return A;

            // recursive situation
        } else {

            // two sub-arrays for temp
            int[] former = new int[length / 2];
            int[] latter = new int[length - length / 2];

            // copy value into the temp
            for (int i = 0; i < length / 2; i++) {
                former[i] = A[i];
            }
            for (int i = 0; i < length - length / 2; i++) {
                latter[i] = A[i + length / 2];
            }

            // mergesort recursively
            former = mergeSort(former);
            latter = mergeSort(latter);

            // merge two sorted sub-arrays into result
            int i = 0, j = 0, k = 0;
            int[] res = new int[length];
            while (k < length) {
                if (i < length / 2 && j < length - length / 2) {
                    if (former[i] < latter[j]) {
                        res[k++] = former[i++];
                    } else {
                        res[k++] = latter[j++];
                    }
                } else if (i < length / 2) {
                    res[k++] = former[i++];
                } else {
                    res[k++] = latter[j++];
                }
            }

            // return result
            return res;
        }
    }
}