package cly753.process;

import java.awt.image.BufferedImage;

import cly753.dodoopmapreduce.NotFeatureWritable;

public class NotProcess {
	public enum IMG_TYPE {JPG};
	public static NotFeatureWritable process(BufferedImage bi, IMG_TYPE type) {
		MyImage image = new MyImage(bi, "jpg");
		
    	NotAlgoInterface algo = NotAlgoLoader.getAlgo();
    	int ret = algo.process(image.data, image.width, image.height, BufferedImage.TYPE_3BYTE_BGR);
	 	
	 	NotFeatureWritable result = new NotFeatureWritable("feature " + ret);
		return result;
	}
}
