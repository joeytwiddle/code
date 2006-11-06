package org.neuralyte.binarydiff;

import org.neuralyte.common.FileUtils;

import java.util.Vector;
import java.io.File;

/** joey Jun 15, 2006 4:37:13 PM */
public class BinaryDiff {

    public static void main(String[] args) {

        DataSource[] inputs = readFiles(args);

        PartialSumMap data = new PartialSumMap(64,inputs);

        for (int j=0;j<inputs.length;j++) {
            data.readPartialSums(j);
        }

        data.result.outputCompressed();

    }

    private static DataSource[] readFiles(String[] args) {
        Vector inputs = new Vector();

        for (int i=0;i<args.length;i++) {

            try {
                File file = new File(args[i]);
                // if (file.exists()) {
                    String content = FileUtils.readStringFromFile(file);
                    DataSource dataSource = new DataSource(file,content,i);
                    inputs.add(dataSource);
                // }
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }

        }
        return (DataSource[])inputs.toArray(new DataSource[inputs.size()]);
    }

}
