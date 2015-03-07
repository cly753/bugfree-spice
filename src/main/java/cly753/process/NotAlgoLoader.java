package cly753.process;

import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.nio.file.Paths;

import cly753.configure.NotConfigure;

public class NotAlgoLoader {
	public static NotAlgoInterface load(String className, String classPath) {		
		try {
			URL url = Paths.get(classPath).toUri().toURL();

			URL[] urls = new URL[]{url};

			ClassLoader cl = URLClassLoader.newInstance(urls);
			Class<?> algoClass= cl.loadClass(className);

			Object o = algoClass.newInstance();
			if (o instanceof NotAlgoInterface)
				return (NotAlgoInterface)o;
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
    		algo = NotAlgoLoader.load(NotConfigure.algoClassName, NotConfigure.algoPath);
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