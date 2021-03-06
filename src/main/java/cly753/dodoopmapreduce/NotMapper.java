package cly753.dodoopmapreduce;

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

import cly753.process.NotProcess;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class NotMapper extends Mapper<Text, BytesWritable, Text, NotFeatureWritable> {	
    private static final Logger logger = LoggerFactory.getLogger(NotMapper.class);

    @Override
    protected void setup(Context context) throws IOException, InterruptedException {

    }
    
    @Override
    public void map(Text key, BytesWritable value, Context context) throws IOException, InterruptedException {
    	BufferedImage tempImage = getBufferedImage(value);
    	//////////////////////////////////////////////////////////
    	// do processing here
    	NotFeatureWritable result = NotProcess.process(tempImage, NotProcess.IMG_TYPE.JPG);
        logger.debug(" key: " + key + ", result: " + result.toString());
        //////////////////////////////////////////////////////////
        context.write(key, result);
    }

    public static byte[] serialize(Writable writable) throws IOException {
    	ByteArrayOutputStream out = new ByteArrayOutputStream();
        DataOutputStream dataOut = new DataOutputStream(out);
        
        writable.write(dataOut);
        dataOut.close();
        
        byte[] result = out.toByteArray();
        return result;
    }
    
    public static BufferedImage getBufferedImage(BytesWritable value) throws IOException {        
        InputStream in = new ByteArrayInputStream(value.getBytes());
        BufferedImage image = ImageIO.read(in);
        in.close();
        return image;
    }
}
