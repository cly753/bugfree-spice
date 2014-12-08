public class NotOutputFormat<Text, NotFeatureWritable> extends FileOutputFormat {

    @Override
    public RecordWriter<Text, NotFeatureWritable> getRecordWriter(FileSystem ignored, JobConf job, String name, Progressable progress) throws IOException {
        Path file = FileOutputFormat.getTaskOutputPath(job, name);
        FileSystem fs = file.getFileSystem(job);
        FSDataOutputStream fileOut = fs.create(file, progress);
        return new NotOutputFormat<Text, NotFeatureWritable>(fileOut);
    }

    protected static class NotFeatureWritableWriter<K, V> implements RecordWriter<K, V> {
        private DataOutputStream out;

        public void NotFeatureWritableWriter(DataOutputStream out) throws IOException {
            this.out = out;
        }

        @Override
        public synchronized void write(Text key, NotFeatureWritable value) throws IOException {
            if (key == null || value == null)
                return ;

            value.write((DataOutput) out);
        }

        @Override
        public synchronized void close(Reporter reporter) throws IOException {
            out.close();
        }
    }
}