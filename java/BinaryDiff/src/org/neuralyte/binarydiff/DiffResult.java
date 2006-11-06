package org.neuralyte.binarydiff;

/** joey Jun 15, 2006 7:28:49 PM */
public class DiffResult {

    PartialSumMap data;

    public DiffResult(PartialSumMap data) {
        this.data = data;
    }

    public void outputCompressed() {
        for (int filenum=0;filenum<data.getDataSourceCount();filenum++) {
            DataSource dataSource = data.getDataSource(filenum);
            // Start at byte 0
            for (int i=0;i<dataSource.getLength();i++) {
                java.util.List blocks = data.isBlockCovered();
            }
        }
    }

}
