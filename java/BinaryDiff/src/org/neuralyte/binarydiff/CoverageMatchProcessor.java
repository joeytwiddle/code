package org.neuralyte.binarydiff;

/** joey Jun 15, 2006 8:18:50 PM */
public class CoverageMatchProcessor implements MatchProcessor {

    DataSource[] dataSources;
    boolean[][] coverage;

    public CoverageMatchProcessor(DataSource[] dataSources) {
        this.dataSources = dataSources;
        coverage = new boolean[dataSources.length][];
        for (int i=0;i<dataSources.length;i++) {
            coverage[i] = new boolean[dataSources[i].getLength()];
            for (int j=0;j<dataSources[i].getLength();j++) {
                coverage[i][j] = false;
            }
        }
    }

    public void processMatch(Block thisBlock, Block otherBlock) {
        // for (int i=0;i<)
    }

}
