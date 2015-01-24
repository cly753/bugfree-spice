package rose.cppnative;

public class BFLoG_API {
	static {
		System.loadLibrary("bflog_api");
	}
	
	public native int Extract(byte[] image, int width, int height);
	
	/*public static void main(String[] args) {
		byte[] test = new byte[400 * 300];
		for(int i = 0; i < 400 * 300; i++)
			test[i] = (byte) (Math.random() * 255);
		
		byte[] ret = new BFLoG_API().Extract(test, 400, 300);
		if(ret == null)
			System.out.println("null");
		else
			System.out.println(ret.length);

	}*/
}
