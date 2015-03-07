package cly753.configure;

import java.util.ArrayList;
import java.util.List;

public class NotConfigure {
	public enum ALGO_TYPE {CPP, JAVA};
	public static String inPath;
	public static String outPath;
	public static ALGO_TYPE type;
	public static String algoPath;
	public static String algoClassName;
	
	public static List<String> error;
	
	public static boolean init(String args[]) {
		error = new ArrayList<String>();
		if (args.length < 4) {
			error.add("Not enough argument. 1}input path 2}output path 3}language of algorithm 4}path of algorithm");
			return false;
		}
		
		inPath = args[0];
		outPath = args[1];
		switch (args[2]) {
		case "CPP": case "C++": case "cpp": case "c++":
			type = ALGO_TYPE.CPP;
			break;
		case "JAVA": case "Java": case "java":
			type = ALGO_TYPE.JAVA;
			
			if (args.length < 5) {
				error.add("Not enough argument. 5}full algorithm class name");
				return false;
			}
			algoClassName = args[5];
			break;
		default:
			error.add("Not support language.");
			return false;
		}
		
		algoPath = args[3];
		return true;
	}
}
