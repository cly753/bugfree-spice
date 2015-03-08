package cly753.process;

import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.nio.file.Paths;

import cly753.configure.NotConfigure;

public class NotAlgoLoader {
	public static final String LABEL = "%%% cly753.process.NotAlgoLoader %%%";
	
	public static NotAlgoInterface load(String[] jarPaths, String className) {
//    	System.out.println(LABEL + "System::java.library.path : " + System.getProperty("java.library.path"));
//    	System.out.println(LABEL + "Job::java.library.path : " + NotConfigure.conf.get("java.library.path"));
		try {	
			URL[] urls = new URL[jarPaths.length];
			for (int i = 0; i < jarPaths.length; i++)
				urls[i] = Paths.get(jarPaths[i]).toUri().toURL();
			
			System.out.println("%%% loading " + className + " at ");
			for (URL u : urls)
				System.out.println("%%% \t" + u.toString());
			
			ClassLoader cl = URLClassLoader.newInstance(urls, NotAlgoInterface.class.getClassLoader());
			Class<?> algoClass = Class.forName(className, true, cl);
			
			Object o = algoClass.newInstance();
			if (o instanceof cly753.process.NotAlgoInterface)
				return (cly753.process.NotAlgoInterface)o;
		} catch (MalformedURLException | ClassNotFoundException | InstantiationException | IllegalAccessException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public static NotAlgoInterface load(String libPath) {
		try {
			NotAlgoInterface algo = new NotAlgoCpp();
			return algo;
		} catch (UnsatisfiedLinkError e) {
			e.printStackTrace();
		}
		return null;
	}

	public static NotAlgoInterface getAlgo() {
		NotAlgoInterface algo = null;
		
    	switch (NotConfigure.type) {
    	case JAVA:
    		String[] algoPaths = new String[]{NotConfigure.algoPath, NotConfigure.algoAPIPath};
    		algo = NotAlgoLoader.load(algoPaths, NotConfigure.algoClassName);
    		break;
    	case CPP:
    		algo = NotAlgoLoader.load(NotConfigure.algoPath);
    		break;
    	default:
    		break;
    	}
    	
		return algo;
	}
}

// load class
// http://www.mkyong.com/java/how-to-load-classes-which-are-not-in-your-classpath/
// http://stackoverflow.com/questions/11016092/how-to-load-classes-at-runtime-from-a-folder-or-jar

// classes should be loaded from same class loader
// http://stackoverflow.com/questions/2614989/brainstorming-weird-jpa-problem-possibly-classpath-or-jar-versioning-problem/2615014#2615014
