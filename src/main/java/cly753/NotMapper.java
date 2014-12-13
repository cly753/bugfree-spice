package cly753;

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;

import javax.imageio.ImageIO;

import org.apache.hadoop.io.BytesWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.mapreduce.Mapper;

public class NotMapper extends Mapper<Text, BytesWritable, Text, NotFeatureWritable> {

    @Override
    protected void setup(Context context) throws IOException, InterruptedException {

    }
    
    @Override
    public void map(Text key, BytesWritable value, Context context) throws IOException, InterruptedException {
        byte[] tempValue = serialize(value);
        InputStream in = new ByteArrayInputStream(tempValue);
        BufferedImage image = ImageIO.read(in);
        in.close();

        // do processing here
        NotFeatureWritable result = new NotFeatureWritable("I am a result of #" + key.toString() + "#");
        //
                
        context.write(key, result);
    }

    public static byte[] serialize(Writable writable) throws IOException {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        DataOutputStream dataOut = new DataOutputStream(out);
        writable.write(dataOut);
        dataOut.close();
        return out.toByteArray();
    }
}
