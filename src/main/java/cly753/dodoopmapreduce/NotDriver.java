package cly753.dodoopmapreduce;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import cly753.configure.NotConfigure;
import cly753.process.NotAlgoLoader;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Date;

public class NotDriver {
    static enum RecordCounters { IMAGE_SUBMITTED, IMAGE_PROCESSED };
    private static final Logger logger = LoggerFactory.getLogger(NotDriver.class);
    
    public static final String LABEL = "%%%% NotDriver : ";

    public static Configuration conf;
    
    public static void main(String[] args) throws Exception {
    	if (!NotConfigure.init(args)) {
            NotConfigure.error.forEach(logger::warn);
    		return ;
    	}

    	conf = new Configuration();

        String uri = conf.get("fs.defaultFS");
        logger.info("fs.defaultFS : " + uri);
        logger.info("System::java.library.path : " + System.getProperty("java.library.path"));
        logger.info("Job::java.library.path : " + conf.get("java.library.path"));

    	Job job = Job.getInstance(conf, "not-a-job");
//		job.addCacheFile(new URI(uri + "/lib/libBFLoG.so#libBFLoG.so"));
//		job.createSymlink();
//		job.addCacheFile(new URI(uri + "/lib/libbflog_api.so#libbflog_api.so"));
//		job.createSymlink();
//    	URI temp = new URI(uri + "/lib/libbflog_api.so#libbflog_api.so");
//    	System.out.println(LABEL + temp.toString());
//    	DistributedCache.createSymlink(conf); DistributedCache.addCacheFile(temp, conf);

        job.setJarByClass(NotMapper.class);  // no need copy jar any more
        job.setInputFormatClass(NotInputFormat.class);
        job.setMapperClass(NotMapper.class);
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(NotFeatureWritable.class);

//		job.setCombinerClass(NotCombiner.class);
//		job.setPartitionerClass(NotPartitioner.class);
//		
//		job.setReducerClass(NotReducer.class);
//		job.setOutputKeyClass(Text.class);
//		job.setOutputValueClass(NotFeatureWritable.class);
        
        job.setOutputFormatClass(NotOutputFormat.class);

        job.setNumReduceTasks(0); // directly write to file system, without calling reducer
        
        job.setSpeculativeExecution(true);

        if (false) {
            logger.warn("Deleting old output path: " + "[put path here]" + "...");
            FileSystem fs = FileSystem.get(new Configuration());
            fs.delete(new Path(NotConfigure.outPath), true);
        }
        
        FileInputFormat.addInputPath(job, new Path(NotConfigure.inPath)); // provide input directory
        FileOutputFormat.setOutputPath(job, new Path(NotConfigure.outPath));

        long start = new Date().getTime();
        boolean ok = job.waitForCompletion(true);
        long end = new Date().getTime();
        System.out.println("Job took " + (end - start) + " milliseconds.");
    }
}

// set mapreduce.framework.name to "local" / "classic" / "yarn"


// many small input files
// http://yaseminavcular.blogspot.in/2011/03/many-small-input-files.html

// The Small Files Problem
// http://blog.cloudera.com/blog/2009/02/the-small-files-problem/