public class Problem_2_d {
    public static void main(String[] args) {
        int[] A = { 2, 3, 8, 6, 1 }; // initialize test array
        System.out.println(mergeSort(A));
    }

    private static int mergeSort(int[] A) {
        int length = A.length;

        // basic case
        if (length == 1) {
            return 0;

            // recursive case
        } else {

            // first-half subarray
            int[] former = new int[length / 2];
            for (int i = 0; i < length / 2; i++) {
                former[i] = A[i];
            }

            // second-half subarray
            int[] latter = new int[length - length / 2];
            for (int i = 0; i < length - length / 2; i++) {
                latter[i] = A[i + length / 2];
            }

            // recursive sort respectively
            int formerCount = mergeSort(former);
            int latterCount = mergeSort(latter);

            // merge
            int i = 0, j = 0, k = 0, res = 0;
            while (i < former.length && j < latter.length) {
                if (former[i] < latter[j]) {
                    A[k++] = former[i++];
                } else {
                    res += (former.length - i); // element in the latter-half inverses with the one in former-half
                    A[k++] = latter[j++];
                }
            }

            // leftover elements
            if (i == former.length) {
                while (j < latter.length) {
                    A[k++] = latter[j++];
                }
            } else {
                while (i < former.length) {
                    A[k++] = former[i++];
                }
            }

            // inversions inside subarrays
            // and crossing subarrays
            return formerCount + latterCount + res;
        }
    }
}
