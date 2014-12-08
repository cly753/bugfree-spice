package cly753;

import java.io.IOException;

import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapreduce.RecordWriter;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.Progressable;

public class NotOutputFormat<Text, NotFeatureWritable> extends FileOutputFormat {

    @Override
    public RecordWriter<Text, NotFeatureWritable> getRecordWriter(FileSystem ignored, JobConf job, String name, Progressable progress) throws IOException {
        Path file = FileOutputFormat.getTaskOutputPath(job, name);
        FileSystem fs = file.getFileSystem(job);
        FSDataOutputStream fileOut = fs.create(file, progress);
        return new NotOutputFormat<Text, NotFeatureWritable>(fileOut);
    }
}