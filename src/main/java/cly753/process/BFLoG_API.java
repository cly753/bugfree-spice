package cly753.process;

public class BFLoG_API {
	private static final String HERE = System.getProperty("user.dir") + "/";
	static {
		System.out.println("=== library path === ");
		System.out.println(System.getProperty("java.library.path"));
		System.out.println("=== ------------ === ");

		System.loadLibrary("bflog_api");
	}
	
	public native int Extract(byte[] image, int width, int height);
	
	// public static void main(String[] args) {
	// 	// byte[] test = new byte[400 * 300];
	// 	// for(int i = 0; i < 400 * 300; i++)
	// 	// 	test[i] = (byte) (Math.random() * 255);
		
	// 	MyImage aImg = new MyImage(HERE + "img.png");
	// 	int ret = new BFLoG_API().Extract(aImg.data, aImg.width, aImg.height);
	// 	System.out.println("ret: " + ret);
	// }
}


