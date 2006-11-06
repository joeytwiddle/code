package org.neuralyte.binarydiff;

import java.util.Iterator;
import java.util.List;

/** joey Jun 15, 2006 6:28:59 PM */
public class BlockMatchDB {

    DataSource[] inputs;
    int resolution = 1;
    byte partialSums[][];

    public BlockMatchDB(int resolution, DataSource[] inputs) {
        this.resolution = resolution;
        this.inputs = inputs;
        partialSums = new byte[inputs.length][];
    }

    public int getDataSourceCount() {
        return inputs.length;
    }

    public DataSource getDataSource(int i) {
        return inputs[i];
    }

    public byte[] generatePartialSums(int j) {
        DataSource dateSource = inputs[j];
        partialSums[j] = generatePartialSums(dateSource);
        return partialSums[j];
    }

    public byte[] generatePartialSums(DataSource dateSource) {
        int length = dateSource.getLength();
        byte[] partialSums = new byte[length];
        byte sum = 0;
        for (int i=0;i<length;i+=resolution) {
            sum += dateSource.getByteAt(i);
            partialSums[i] = sum;
        }
        return partialSums;
    }

    public void doScan(int windowSize, int scanResolution) {
        int scanningSource = 0;
        int atPosition = 0;
        // DataSource ds = inputs[scanningSource];
        while (true) {
            byte sumForBlock = getSumForBlock(scanningSource,atPosition,windowSize);
        }
    }

    private byte getSumForBlock(int ds, int atPosition, int windowSize) {
        return (byte)(partialSums[ds][atPosition+windowSize] - partialSums[ds][atPosition]);
    }

    int scanSize = 40;

    public void buildInitialMatchesDB(int scanSize) {
        ListMap matchesDB = new ListMap();
        for (int i=0;i<getDataSourceCount();i++) {
            DataSource ds = getDataSource(i);
            for (int j=0;j<ds.getLength()-scanSize;j++) {
                byte sum = (byte)( partialSums[i][j+scanSize] - partialSums[i][j] );
                List list = matchesDB.getListFor(new Byte(sum));
                // This is a list of earlier blocks with the same checksum, some of which may match this block.

                // Try to find the earliest actual match:
                boolean matchFound = false;
                for (int k=0;k<list.size();k++) {
                    int[] earlierBlock = (int[])list.get(k);
                    boolean reallyMatches = checkMatch(i,j,earlierBlock[0],earlierBlock[1],scanSize);
                    if (reallyMatches) {
                        // @todo: Would be good to try to expand this block first.
                        output.copyEarlierBlock(earlierBlock[0],earlierBlock[1]);
                        matchFound = true;
                        break;
                    }
                }
                if (!matchFound) {
                    output.writeByte(ds.getByteAt(j));
                }

                int[] block = { i, j };
                list.add(block);
            }
        }
        Iterator sumIterator = matchesDB.keySet().iterator();
        while (sumIterator.hasNext()) {
            Integer sum = (Integer)sumIterator.next();
            List list = matchesDB.getListFor(sum);
            System.out.println("Checksum "+sum+" has "+list.size()+" occurrences.");
        }
    }

    boolean checkMatch(int sourceA, int posA, int sourceB, int posB, int length) {
        // A clever thing to do here might be a breadth-first descending binary search
        // This might detect a failed match faster than a simple linear search,
        // although it will have a cost in terms of recursion and the stack.

        // Simple linear search
        for (int i=0;i<length;i++) {
            // This could be implemented using differences between partial sums, to avoid referring to the original input data.
            // That would probably be best implemented by calculating the difference between the initial partial-sum of each block first.
            if (inputs[sourceA].getByteAt(posA + i) != inputs[sourceB].getByteAt(posB + i)) {
                return false;
            }
        }
        return true;
    }

    // MatchProcessor matchProcessor = null;
    // matchProcessor = new CoverageMatchProcessor(inputs);

    /*
    ListMap listMap = new ListMap();
    DiffResult result = null;

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

    // For DiffResult:

    private Iterator itarateAllMatches() {
        return this.listMap.entrySet().iterator();
    }

    public List isBlockCovered() {
//        Iterator i = itarateAllMatches();
//        while (i.hasNext()) {
//            List l = (List)i.next();
//            if (l.size() == 1) {
//                continue;
//            } else {
//                for (int i=)
//            }
//        }
        return null; // no matches =/
        if (matchProcessor != null) {
            if (matchProcessor instanceof CoverageMatchProcessor) {
                // return ((CoverageMatchProcessor)matchProcessor).isBlockCovered()
            }
        }
        return null;
    }

    */

}
