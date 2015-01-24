import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class Wrap {
	public native void sayHi();
	public native int process(byte[] image, int width, int height);

	private static final String HERE;

	static {
		HERE = System.getProperty("user.dir") + "/";
		System.load(HERE + "prog.so");
	}

	public static void main(String[] args) {
		MyImage aImg = new MyImage(HERE + "img.png");
		if (aImg.ok)
			System.out.println(new Wrap().process(aImg.data, aImg.width, aImg.height));
	}
}

class MyImage {
	public boolean ok = true;
	public byte data[];
	public int width;
	public int height;

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
		}
		catch (Exception e) {
			System.out.println(e.toString());

			this.ok = false;
			this.data = null;
			this.width = 0;
			this.height = 0;
		}
	}
}
