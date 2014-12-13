package cly753;

import java.io.IOException;
import java.util.Iterator;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.output.MultipleOutputs;


public class NotReducer extends Reducer<Text, NotFeatureWritable, Text, NotFeatureWritable> {
    private MultipleOutputs<Text, NotFeatureWritable> multipleOutputs;

    @Override
    protected void setup(Context context) throws IOException, InterruptedException {
        multipleOutputs = new MultipleOutputs<Text, NotFeatureWritable>(context);
    }

    @Override
    public void reduce(Text key, Iterable<NotFeatureWritable> values, Context context) throws IOException {
    	NotFeatureWritable eachValue = null;
    	Iterator<NotFeatureWritable> iter = values.iterator();
        while (iter.hasNext()) {
        	eachValue = iter.next();
        	
        	try {
        		multipleOutputs.write(new Text("#key field#"), eachValue, key.toString());
			} catch (InterruptedException e) {
				// e.printStackTrace();
				System.out.println(" !! NotReducer.reduce() !! InterruptedException !! ");
			}
        }
    }

    @Override
    protected void cleanup(Context context) throws IOException, InterruptedException {
        multipleOutputs.close();
    }
}