package cly753;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class NotDriver {
    static enum RecordCounters { IMAGE_SUBMITTED, IMAGE_PROCESSED };

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();

        Job job = Job.getInstance(conf, "not a job");

        // job.setJarByClass(WordCount2.class);

        job.setInputFormatClass(NotInputFormat.class);
        job.setMapperClass(NotMapper.class);
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(NotFeatureWritable.class);

        // job.setCombinerClass(NotCombiner.class);
        // job.setPartitionerClass(NotPartitioner.class);

        // job.setReducerClass(NotReducer.class);
        // job.setOutputKeyClass(Text.class);
        // job.setOutputValueClass(IntWritable.class);
        // job.setOutputFormatClass(NotOutputFormat.class);

        
        job.setNumReduceTasks(0); // directly write to file system, without calling reducer
        job.setSpeculativeExecution(true);

        String inputPath = "";
        String outputPath = "";
        String prefix = "";
        for (String arg : args) {
        	if (arg.startsWith("INPUTPATH"))
        		inputPath = arg.substring(9, arg.length());
            if (arg.startsWith("OUTPUTPATH"))
            	outputPath = arg.substring(10, arg.length());
//            if (arg.startsWith("PREFIX"))
//        		prefix = arg.substring(9, arg.length());
        }
        System.out.println("INPUTPATH : " + inputPath);
        System.out.println("OUTPUTPATH: " + outputPath);
        FileInputFormat.addInputPath(job, new Path(inputPath)); // provide input directory
        FileOutputFormat.setOutputPath(job, new Path(outputPath));

        boolean ok = job.waitForCompletion(true);
        // System.exit(ok ? 0 : 1); // .waitFor... will submit the job

        // int imageSubmitted = RunningJob.getCounters(RecordCounters.IMAGE_SUBMITTED);
        // int imageProcessed = RunningJob.getCounters(RecordCounters.IMAGE_PROCESSED);
    }
}

// set mapreduce.framework.name to "local" / "classic" / "yarn"


// many small input files
// http://yaseminavcular.blogspot.in/2011/03/many-small-input-files.html

// The Small Files Problem
// http://blog.cloudera.com/blog/2009/02/the-small-files-problem/