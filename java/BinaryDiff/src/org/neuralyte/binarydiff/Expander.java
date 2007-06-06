package org.neuralyte.binarydiff;

import org.neuralyte.fastbuffers.ByteRingBuffer;

import java.io.*;

/** joey Jul 1, 2006 2:14:54 AM */
public class Expander {

    /** @deprecated Now using BinaryDiff.main(). **/
    public static void main(String[] args) throws Throwable {
        InputStream in = new FileInputStream("bdiff_output");
        expand(in);
    }

    public static void expand(InputStream in) throws IOException {

        in = new BufferedInputStream(in);

        // OutputStream out = new FileOutputStream("expanded.out");
        MultiFileOutputStream out = new MultiFileOutputStream();
        // out = new BufferedOutputStream(out);

        BinaryDiffOptions options = new BinaryDiffOptions();

        if (!options.printNoHeaders) {

            // Read headers:
            while (true) {
                String line = readLineFromInputStream(in);
                if (line.length() == 0) {
                    break; // Done reading headers
                } else if (line.startsWith("File: ")) {
                    int firstSpace = "File: ".length() - 1;
                    int secondSpace = line.indexOf(" ",firstSpace+1);
                    int length = Integer.parseInt(line.substring(firstSpace+1,secondSpace));
                    String fileName = line.substring(secondSpace+1);
                    out.addOutputFile(length,fileName);
                }
            }

        }

        ByteRingBuffer writtenHistory = new ByteRingBuffer(options.maxHistory);

        while (true) {

            int i = in.read();
            if (i == -1) {
                break;
            }

            if (i == 0) {
                // Copy earlier block
                int startPos = readInt(in);
                int length = readShort(in);
                System.out.println(writtenHistory.position+": Copying "+length+" bytes from "+startPos+" ...");
                for (int j=0;j<length;j++) {
                    byte b = writtenHistory.get(startPos+j);
                    writtenHistory.addByte(b);
                    out.write(b);
                }
            } else {
                // Copy i raw bytes
                System.out.println(writtenHistory.position+": Writing "+i+" raw bytes...");
                for (int j=0;j<i;j++) {
                    byte b = (byte)in.read();
                    writtenHistory.addByte(b);
                    out.write(b);
                }
            }

        }

        out.flush();

    }

    private static int readShort(InputStream in) throws IOException {
        int low = fixByte((byte)in.read());
        int hi = fixByte((byte)in.read());
        return ( low + hi*256 );
    }

    private static int readInt(InputStream in) throws IOException {
        int low = fixByte((byte)in.read());
        int hi = fixByte((byte)in.read());
        int hi2 = fixByte((byte)in.read());
        int hi3 = fixByte((byte)in.read());
        return (low) + (hi)*256 + (hi2)*256*256 + (hi3)*256*256*256;
    }

    private static int fixByte(byte b) {
        if (b>=0) {
            return b;
        } else {
            // return 127 - (int)b;
            return 256 + (int)b;
        }
    }

    public static String readLineFromInputStream(InputStream in) throws IOException {
        StringBuffer buffer = new StringBuffer();
        while (true) {
            int i = in.read();
            if (i == -1 || (char)i == '\n') {
                break;
            } else {
                buffer.append((char)i);
            }
        }
        return buffer.toString();
    }

}
