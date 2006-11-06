package org.neuralyte.binarydiff;

/** joey Jun 15, 2006 6:49:32 PM */
public class Block {
    DataSource dataSource;
    int pos;
//    byte sum;

    public Block(DataSource raw, int pos, byte sum) {
        this.dataSource = raw;
        this.pos = pos;
//        this.sum = sum;
    }

    public String toString() {
//        return "Block["+dataSource+","+pos+":"+sum+"]";
        return "Block["+dataSource+","+pos+"]";
    }

    public boolean matchesBlock(Block otherBlock,int scanSize) {
        for (int i=0;i<scanSize;i++) {
            if (dataSource.getByteAt(pos+i) != otherBlock.dataSource.getByteAt(pos+i)) {
                return false;
            }
        }
        return true;
    }
}


