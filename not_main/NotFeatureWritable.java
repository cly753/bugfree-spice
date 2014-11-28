package not_main;

public class NotFeatureWritable implements Writable /* implements WritableComparable */ {

    private String feature = "zhoushen wan sui";

    public void NotFeatureWritable(String feature) {
        this.feature = feature;
    }

    @Override
    void readFields(DataInput in) throws IOException {
        feature = Text.readUTF(in);
    }

    @Override
    void write(DataOutput out) throws IOException {
        Text.writeUTF(out, feature);
    }

    // @Override
    // public int compareTo(NotFeatureWritable that) {
    // }
    // @Override
    // public boolean equals(NotFeatureWritable that) {
    //     if (!(that instanceof NotFeatureWritable))
    //         return false;
    // }
    // @Override
    // public int hashCode() {
    // }

    public String toString() {
        return feature;
    }
}