
public class PI {

	static double func_pi(int j) {
		if (j > 1)
			return func_pi(j-1);	// Replace this line to return a correct value
		else
			return 1;
	}
	
	public static void main(String[] args) {
		double pi = Math.sqrt(func_pi(2000) * 8);
		System.out.printf("PI=%1.16f\n", pi);
	}

}
