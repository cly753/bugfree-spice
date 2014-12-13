package cly753;

import java.io.IOException;

import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.mapreduce.RecordWriter;
import org.apache.hadoop.mapreduce.TaskAttemptContext;
import org.apache.hadoop.mapreduce.TaskID;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class NotOutputFormat<Text, NotFeatureWritable> extends FileOutputFormat {

	@Override
	public RecordWriter getRecordWriter(TaskAttemptContext task)
			throws IOException, InterruptedException {
		System.out.println("\n\n\n## " + getOutputName(task) + "##\n\n\n");

		Path outputPath = FileOutputFormat.getOutputPath(task);
		TaskID taskId = task.getTaskAttemptID().getTaskID();  
		outputPath = new Path(outputPath, "result-r-" + taskId.getId());
		FileSystem fs = outputPath.getFileSystem(task.getConfiguration());
		FSDataOutputStream out = fs.create(outputPath);
		return new NotRecordWriter<Text, NotFeatureWritable>(out);
	}

}

/*
 * reference
 * 
 * [Custom File Output in
 * Hadoop](http://johnnyprogrammer.blogspot.com/2012/01/custom
 * -file-output-in-hadoop.html)
 * 
 * belows are useless =========================================================
 * 
 * [file output
 * committer](http://www.programcreek.com/java-api-examples/index.php
 * ?api=org.apache.hadoop.mapreduce.lib.output.FileOutputCommitter)
 * 
 * [archive-commons
 * ](https://github.com/internetarchive/archive-commons/blob/master
 * /ia-tools/src/main/java/org/archive/hadoop/mapreduce/ZipNumOutputFormat.java)
 * 
 * [
 * MultipleOutputFormat](http://hadoop.apache.org/docs/r0.23.11/api/src-html/org
 * /apache/hadoop/mapred/lib/MultipleOutputFormat.html)
 * 
 * [MultipleOutputs](http://
 * hadoop.apache.org/docs/r2.2.0/api/src-html/org/apache
 * /hadoop/mapreduce/lib/output/MultipleOutputs.html) can use CompressionCodec
 * to compress input/output stream (ignored here)
 * 
 * 
 * issue: cannot create custom filename without Reducer...
 */

