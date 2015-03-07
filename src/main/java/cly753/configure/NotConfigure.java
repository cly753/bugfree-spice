package cly753.configure;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

import cly753.process.NotAlgoLoader;

public class NotConfigure {
	public enum ALGO_TYPE {CPP, JAVA};
	
	public static String inPath; // 1}
	public static String outPath; // 2}
	public static ALGO_TYPE type; // 3}
	public static String algoPath; // 4}
	public static String algoClassName; // optional. 5}
	
	public static List<String> error;
	
	public static boolean init(String args[]) {
		System.out.println("args are: ");
		for (int i = 0; i < args.length; i++) {
			System.out.println(i + ": " + args[i]);
		}
		
		error = new ArrayList<String>();
		if (args.length < 4) {
			error.add("Not enough argument. 1}input path 2}output path 3}language of algorithm 4}path of algorithm");
			return false;
		}
		
		inPath = args[0];
		outPath = args[1];
		algoPath = args[3];
		
		switch (args[2]) {
		case "CPP": case "C++": case "cpp": case "c++":
			type = ALGO_TYPE.CPP;
			Path p = Paths.get(algoPath);
			String libFullName = p.getFileName().toString();
			if (libFullName.length() < 7 || !libFullName.substring(0, 3).equals("lib") || !libFullName.substring(libFullName.length() - 3, libFullName.length()).equals(".so")) {
				error.add("Invalid algorithm library name: " + libFullName);
				return false;
			}
			break;
		case "JAVA": case "Java": case "java":
			if (args.length < 5) {
				error.add("Not enough argument. 5}full algorithm class name");
				return false;
			}
			type = ALGO_TYPE.JAVA;
			algoPath = args[3];
			break;
		default:
			error.add("Language not supported.");
			return false;
		}
		
		if (null == NotAlgoLoader.getAlgo()) {
			error.add("Failed to load algorithm class.");
			return false;
		}
		return true;
	}
}
