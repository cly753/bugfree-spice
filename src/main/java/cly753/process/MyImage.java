package cly753.process;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.File;

import javax.imageio.ImageIO;

public class MyImage {
	private static final Logger logger = LoggerFactory.getLogger(MyImage.class);

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

			logger.debug("Load OK");
		}
		catch (Exception e) {
			System.out.println(e.toString());

			this.ok = false;
			this.data = null;
			this.width = 0;
			this.height = 0;

			logger.error(" Load NOT OK : public MyImage(BufferedImage originalImage) ");
		}
	}

	public MyImage(String path) {
		try {
			logger.debug("path: " + path);
			String temp[] = path.split("\\.");
			String TYPE = temp[temp.length - 1];

			BufferedImage originalImage = ImageIO.read(new File(path));
			ByteArrayOutputStream baos = new ByteArrayOutputStream();

			ImageIO.write(originalImage, TYPE, baos);
			baos.flush();
			this.data = baos.toByteArray();
			baos.close();

			this.width = originalImage.getWidth();
			this.height = originalImage.getHeight();

			logger.debug("Load OK");
		}
		catch (Exception e) {
			System.out.println(e.toString());

			this.ok = false;
			this.data = null;
			this.width = 0;
			this.height = 0;

			logger.error("Load NOT OK : public MyImage(String path)");
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
			logger.error("Load NOT OK : public MyImage(byte[] data, int width, int height)");
		}
	}
}