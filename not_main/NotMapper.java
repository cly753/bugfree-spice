package not_main;

public class NotMapper extends MapReduceBase implements Mapper<Text, BytesWritable, Text, NotFeatureWritable> {

    @Override
    protected void setup(Context context) throws IOException, InterruptedException {

    }
    
    @Override
    public void map(Text key, BytesWritable value, Context context, Reporter reporter) throws IOException {
        reporter.incrCounter(NotDriver.RecordCounters.IMAGE_SUBMITTED, 1);

        byte[] tempValue = serialize(value);
        InputStream in = new ByteArrayInputStream(tempValue);
        BufferedImage image = ImageIO.read(in);
        in.close();

        NotFeatureWritable result = /* ... */(value);

        reporter.incrCounter(NotDriver.RecordCounters.IMAGE_PROCESSED, 1);
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
