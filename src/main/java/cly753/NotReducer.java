//package cly753;
//
//import java.io.IOException;
//import java.util.Iterator;
//
//import org.apache.hadoop.io.NullWritable;
//import org.apache.hadoop.io.Text;
//import org.apache.hadoop.mapreduce.Reducer;
//import org.apache.hadoop.mapreduce.lib.output.MultipleOutputs;
//
//public class NotReducer extends Reducer<Text, NotFeatureWritable, Text, NotFeatureWritable> {
//    private MultipleOutputs<Text, NotFeatureWritable> multipleOutputs;
//
//    @Override
//    protected void setup(Context context) throws IOException, InterruptedException {
//        multipleOutputs = new MultipleOutputs<Text, NotFeatureWritable>(context);
//    }
//
//    public void reduce(Text key, Iterator<NotFeatureWritable> values, Context context) throws IOException {
////         context.write(key, values);
//
//        for (NotFeatureWritable eachFeature : values)
//            multipleOutputs.write(key.toString(), null, eachFeature);
//    }
//
//    @Override
//    protected void cleanup(Context context) throws IOException, InterruptedException {
//        multipleOutputs.close();
//    }
//}