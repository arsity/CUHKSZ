public class MagicSquare {
	static final int N = 9;
	static final int K = 3;
	static final int constant = 15;

	static int[] arr = new int[N];

	static void printSquare() {
		for (int i = 0; i < N; i++) {
			System.out.print(arr[i] + " ");

			if ((i + 1) % K == 0)
				System.out.println();
		}
		System.out.println("=====");
	}

	static boolean isAns() {
		int sum;

		for (int r = 0; r < K; r++) {
			sum = 0;
			for (int c = 0; c < K; c++)
				sum += arr[c * K + r];
			if (sum != constant)
				return false;
		}

		for (int c = 0; c < K; c++) {
			sum = 0;
			for (int r = 0; r < K; r++)
				sum += arr[c * K + r];
			if (sum != constant)
				return false;
		}

		sum = 0;
		for (int i = 0; i < N; i += K + 1)
			sum += arr[i];
		if (sum != constant)
			return false;

		sum = 0;
		for (int i = K - 1; i < N - 1; i += K - 1)
			sum += arr[i];
		if (sum != constant)
			return false;

		return true;
	}

	static boolean isUsed(int p, int n) {
		for (int i = 0; i < p; i++)
			if (arr[i] == n)
				return true;
		return false;
	}

	static void solveSquare(int p) {

		/* add your code here */
		if (p==N&&isAns()) {
			printSquare();
		} else if (p < N) {
			for (int i = 1; i < 10; i++) {
				if (!isUsed(p, i)) {
					arr[p] = i;
					solveSquare(p + 1);
				}
			}
		} else {
			

	}

	public static void main(String[] args) {
		solveSquare(0);
	}

}
