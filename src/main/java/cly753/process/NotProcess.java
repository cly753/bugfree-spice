package cly753.process;

import java.awt.image.BufferedImage;

import cly753.NotFeatureWritable;
import cly753.configure.NotConfigure;

public class NotProcess {
	public enum IMG_TYPE {JPG};
	public static NotFeatureWritable process(BufferedImage bi, IMG_TYPE type) {
		MyImage aImg = new MyImage(bi, "jpg");
    	int ret = -1;
    	
    	switch (NotConfigure.type) {
    	case JAVA:
    		break;
    	case CPP:
    		ret = new BFLoG_API().Extract(aImg.data, aImg.width, aImg.height);
    		break;
    	default:
    		break;
    	}
	 	
	 	NotFeatureWritable result = new NotFeatureWritable("feature " + ret);
		return result;
	}
}
