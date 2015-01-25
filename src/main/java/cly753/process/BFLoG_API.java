package cly753.process;

public class BFLoG_API {
//	private static final String HERE = System.getProperty("user.dir") + "/";
	
	static {
//		http://mail-archives.apache.org/mod_mbox/hadoop-common-user/200904.mbox/%3CC61F954B.2197%25heyongqiang@software.ict.ac.cn%3E
//		String oldPath=System.getProperty("java.library.path");
//		System.setProperty("java.library.path", oldPath==null?local_path_of_lib_file:oldPath+"/" +local_path_of_lib_file))
//		System.loadLibrary("XXX");
		
//		System.out.println("=== library path === ");
//		System.out.println(System.getProperty("java.library.path"));
//		System.out.println("=== ------------ === ");

		System.loadLibrary("bflog_api");
//		System.load("hdfs://localhost:9000/lib/libbflog_api.so");
	}
	
	public native int Extract(byte[] image, int width, int height);
}