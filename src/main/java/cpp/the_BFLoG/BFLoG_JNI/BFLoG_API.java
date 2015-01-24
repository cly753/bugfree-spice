import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class BFLoG_API {
	static {
		System.out.println("=== library path === ");
		System.out.println(System.getProperty("java.library.path"));
		System.out.println("=== ------------ === ");

		System.loadLibrary("bflog_api");
	}
	
	public native int Extract(byte[] image, int width, int height);
	
	public static void main(String[] args) {
		// byte[] test = new byte[400 * 300];
		// for(int i = 0; i < 400 * 300; i++)
		// 	test[i] = (byte) (Math.random() * 255);
		
		String HERE = System.getProperty("user.dir") + "/";
		MyImage aImg = new MyImage(HERE + "img.png");
		int ret = new BFLoG_API().Extract(aImg.data, aImg.width, aImg.height);
		System.out.println("ret: " + ret);
	}
}


class MyImage {
	public boolean ok = true;
	public byte data[];
	public int width;
	public int height;

	public MyImage(BufferedImage originalImage, String TYPE) {
		try {
			ByteArrayOutputStream baos = new ByteArrayOutputStream();

			ImageIO.write( originalImage, TYPE, baos );
			baos.flush();
			this.data = baos.toByteArray();
			baos.close();

			this.width = originalImage.getWidth();
			this.height = originalImage.getHeight();

			System.out.println(" Load OK ");
		}
		catch (Exception e) {
			System.out.println(e.toString());

			this.ok = false;
			this.data = null;
			this.width = 0;
			this.height = 0;

			System.out.println(" Load NOT OK : public MyImage(BufferedImage originalImage) ");
		}
	}

	public MyImage(String path) {
		try {
			System.out.println("path: " + path);
			String temp[] = path.split("\\.");
			String TYPE = temp[temp.length - 1];

			BufferedImage originalImage = ImageIO.read(new File(path));
			ByteArrayOutputStream baos = new ByteArrayOutputStream();

			ImageIO.write( originalImage, TYPE, baos );
			baos.flush();
			this.data = baos.toByteArray();
			baos.close();

			this.width = originalImage.getWidth();
			this.height = originalImage.getHeight();

			System.out.println(" Load OK ");
		}
		catch (Exception e) {
			System.out.println(e.toString());

			this.ok = false;
			this.data = null;
			this.width = 0;
			this.height = 0;

			System.out.println(" Load NOT OK : public MyImage(String path) ");
		}
	}

	public MyImage(byte[] data, int width, int height) {
		if (data != null) {
			this.data = data;
			this.width = width;
			this.height = height;
		}
		else {
			this.ok = false;
			this.data = null;
			this.width = 0;
			this.height = 0;
			System.out.println(" Load NOT OK : public MyImage(byte[] data, int width, int height)");
		}
	}
}
