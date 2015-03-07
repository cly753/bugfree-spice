package cly753.process;

import cly753.configure.NotConfigure;

public class NotAlgoCpp implements NotAlgoInterface {	
	static {
//		System.load("hdfs://localhost:9000/lib/libbflog_api.so");
//		System.loadLibrary(NotConfigure.algoLibName);
		System.load(NotConfigure.algoPath);
	}
	
	public native int Extract(byte[] image, int width, int height, int format);

	@Override
	public int process(byte[] image, int width, int height, int format) {
		int result = Extract(image, width, height, format);
		return result;
	}
}

// reference
// http://mail-archives.apache.org/mod_mbox/hadoop-common-user/200904.mbox/%3CC61F954B.2197%25heyongqiang@software.ict.ac.cn%3E
