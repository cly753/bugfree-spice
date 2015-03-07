package cly753.process;

import java.awt.image.BufferedImage;

import cly753.configure.NotConfigure;
import cly753.dodoopmapreduce.NotFeatureWritable;

public class NotProcess {
	public enum IMG_TYPE {JPG};
	public static NotFeatureWritable process(BufferedImage bi, IMG_TYPE type) {
		MyImage image = new MyImage(bi, "jpg");
    	int ret = -1;
    	
    	switch (NotConfigure.type) {
    	case JAVA:
    		NotAlgoInterface algo = NotAlgoLoader.load(NotConfigure.algoClassName, NotConfigure.algoPath);
    		ret = algo.process(image.data, image.width, image.height, BufferedImage.TYPE_3BYTE_BGR);
    		break;
    	case CPP:
    		ret = new BFLoG_API().Extract(image.data, image.width, image.height);
    		break;
    	default:
    		break;
    	}
	 	
	 	NotFeatureWritable result = new NotFeatureWritable("feature " + ret);
		return result;
	}
}
