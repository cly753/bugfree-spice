package cly753;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.Iterator;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.Writable;

public class NotFeatureWritable implements Writable /* implements WritableComparable */ {

    private String feature = "zhoushen wan sui";

    public NotFeatureWritable(String feature) {
        this.feature = feature;
    }

    @Override
	public void readFields(DataInput in) throws IOException {
        feature = Text.readString(in);
    }

    @Override
    public void write(DataOutput out) throws IOException {
        Text.writeString(out, feature);
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