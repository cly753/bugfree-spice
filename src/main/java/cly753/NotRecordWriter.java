package cly753;

import java.io.IOException;

import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.RecordWriter;
import org.apache.hadoop.mapreduce.TaskAttemptContext;

public class NotRecordWriter<Text, NotFeatureWritable> extends
		RecordWriter<Text, NotFeatureWritable> {
	private static final String ENCODING = "UTF-8";

	private FSDataOutputStream out;

	public NotRecordWriter(FSDataOutputStream out) throws IOException {
		this.out = out;
	}

	public synchronized void write(Text key, NotFeatureWritable value)
			throws IOException {
		if (key == null || value == null) {
			out.write("## NotRecordWriter: ERROR: null key or value ##"
					.getBytes(ENCODING));
			return;
		}

		String toWrite = ("##key: " + key.toString() + "## ##value: "
				+ value.toString() + "##");
		out.write(toWrite.getBytes(ENCODING));
	}

	@Override
	public void close(TaskAttemptContext arg0) throws IOException,
			InterruptedException {
		synchronized (this) {
			out.close();
		}
	}
}
