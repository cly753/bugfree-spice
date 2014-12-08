package not_main;

public class NotReducer extends MapReduceBase implements Reducer<Text, NotFeatureWritable, Text, NotFeatureWritable> {

    private MultipleOutputs<NullWritable, Text> multipleOutputs;

    @Override
    protected void setup(Context context) throws IOException, InterruptedException {
        multipleOutputs = new MultipleOutputs<NullWritable, Text>(context);
    }

    @Override
    public void reduce(Text key, Iterator<NotFeatureWritable> values, OutputCollector<Text, NotFeatureWritable> output, Reporter reporter) throws IOException {

        // context.write(key, values);

        for (NotFeatureWritable eachFeature : values)
            multipleOutputs.write(NullWritable.get(), eachFeature.toString(), key.toString());
    }

    @Override
    protected void cleanup(Context context) throws IOException, InterruptedException {
        multipleOutputs.close();
    }
}