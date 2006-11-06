package org.neuralyte.binarydiff;

import java.util.Iterator;
import java.util.List;

/** joey Jun 15, 2006 6:28:59 PM */
public class PartialSumMap {

    DataSource[] inputs;
    int scanSize;

    MatchProcessor matchProcessor = null;

    public PartialSumMap(int scanSize, DataSource[] inputs) {
        this.scanSize = scanSize;
        this.inputs = inputs;
        matchProcessor = new CoverageMatchProcessor(inputs);
    }

    ListMap listMap = new ListMap();
    DiffResult result = null;

    public void readPartialSums(int j) {
        DataSource dateSource = inputs[j];
        if (dateSource.getLength() < scanSize) {
            System.out.println("Source "+dateSource+" is too small for scan "+scanSize);
            return;
        }
        int length = dateSource.getLength() - scanSize;
        // byte[] partialSums = new byte[length];
        byte sum = 0;
        // Sum the first scanSize-1 bytes:
        for (int i=0;i<scanSize-1;i++) {
            sum += dateSource.getByteAt(i);
        }
        // partialSums[0] = sum; // The sum of the first scanSize bytes
        // for (int i=1;i<length;i++) {
            // sum += dateSource.getByteAt(scanSize+i-1);
            // partialSums[i] = sum;
        // }
        for (int i=0;i<length;i++) {
            if (i % 1024 == 0) {
                System.out.println("Scanning "+dateSource+" at "+i);
            }
            sum += dateSource.getByteAt(scanSize-1+i);
            // partialSums[i] = sum;
            dataFindMatch(dateSource,i,sum);
        }
    }
    public void dataFindMatch(DataSource raw, int pos, byte sum) {
        Block thisBlock = new Block(raw,pos,sum);
//        System.out.println("Created block "+thisBlock);
        List list = listMap.getListFor(new Byte(sum));
        for (int i=0;i<list.size();i++) {
            Block otherBlock = (Block)list.get(i);
//            System.out.println(thisBlock+" has the same cksum as "+otherBlock);
            // Compare the two blocks
            if (thisBlock.matchesBlock(otherBlock,scanSize)) {
                // ...
                if (matchProcessor != null) {
                    matchProcessor.processMatch(thisBlock,otherBlock);
                }
            } else {
                System.out.println("Match "+thisBlock+" and "+otherBlock+" was a false-positive.");
            }
        }
        list.add(thisBlock);
    }

    public int getDataSourceCount() {
        return 0;
    }

    public DataSource getDataSource(int i) {
        return inputs[i];
    }

    // For DiffResult:

    public List isBlockCovered() {
        /*
        Iterator i = itarateAllMatches();
        while (i.hasNext()) {
            List l = (List)i.next();
            if (l.size() == 1) {
                continue;
            } else {
                for (int i=)
            }
        }
        return null; // no matches =/
        */
        if (matchProcessor != null) {
            if (matchProcessor instanceof CoverageMatchProcessor) {
                // return ((CoverageMatchProcessor)matchProcessor).isBlockCovered()
            }
        }
        return null;
    }

    private Iterator itarateAllMatches() {
        return this.listMap.entrySet().iterator();
    }

}
