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

import cly753.process.BFLoG_API;
import cly753.process.MyImage;

public class NotMapper extends Mapper<Text, BytesWritable, Text, NotFeatureWritable> {
	
    @Override
    protected void setup(Context context) throws IOException, InterruptedException {

    }
    
    @Override
    public void map(Text key, BytesWritable value, Context context) throws IOException, InterruptedException {
    	BufferedImage tempImage = getBufferedImage(value);
    	System.out.println("%%%% type = " + tempImage.getType());
    	//////////////////////////////////////////////////////////
    	// do processing here
    	ByteArrayOutputStream baos = new ByteArrayOutputStream();
    	ImageIO.write( tempImage, "jpg", baos );
    	
    	baos.flush();
    	byte[] imageInByte = baos.toByteArray();
    	baos.close();
    	
    	MyImage aImg = new MyImage(imageInByte, tempImage.getWidth(), tempImage.getHeight());
    	int ret = -1;
	 	ret = new BFLoG_API().Extract(aImg.data, aImg.width, aImg.height);
        NotFeatureWritable result = new NotFeatureWritable("I am a result of #" + key.toString() + "# feature: " + ret);
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
