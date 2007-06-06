package org.neuralyte.binarydiff;

import java.io.*;
import java.util.Vector;

/** joey Jun 15, 2006 4:37:13 PM */
public class BinaryDiff {
    
    static BinaryDiffOptions options = new BinaryDiffOptions();
    
    public static void showHelpAndExit() {
            System.out.println("");
            System.out.println("BinaryDiff [-scansize <num>] [-history <num>] <archive_name> <file_name>s...");
            System.out.println("BinaryDiff -extract <archive_name>");
            // System.out.println("BinaryDiff -verify <archive_name>   TODO!"); // @todo
            System.out.println("");
            System.out.println("  -scansize <num> specifies the minimum size of duplicate blocks, default "+options.scanSize);
            // System.out.println("    Smaller scansize means finer-grained search, but more sum-collisions, so slower processing.");
            // System.out.println("    Larger scansize means faster search, but repeated blocks smaller than this will not be found.");
            //// I found this to be untrue: scan size 8 seems to be faster than 64!
            System.out.println("");
            System.out.println("  -history <num> specifies the amount of past data which is checked for repetition, default "+options.maxHistory);
            System.out.println("    Larger history means better compression, but slower processing, and greater memory usage.");
            System.out.println("");
            System.out.println("  <archive_name> can be - to take input for extraction from stdin, or send output to stdout.");
            System.out.println("    This is useful if you want to pipe to/from gzip/gunzip as you might with a tarball.");
            System.out.println("");
            System.out.println("  <file_name> is a list of the files + directories you want to compress.");
            System.out.println("");
            System.exit(0);
    }

    public static void main(String[] args) throws Throwable {
        
        if (args.length == 0 || args[0].equals("--help")) {
            showHelpAndExit();
        }
        
        if (args.length > 0 && args[0].equals("-extract")) {
            String archiveName = args[1];
            InputStream in = ( archiveName.equals("-") ? System.in : new FileInputStream(new File(archiveName)) );
            Expander.expand(in);
        } else {
            
            int onArg = 0;
            while (onArg < args.length) {
                String arg = args[onArg];
                if (arg.equals("--help")) {
                    showHelpAndExit();
                }
                if (arg.equals("-scansize")) {
                    options.scanSize = Integer.parseInt(args[onArg+1]);
                    onArg += 2;
                } else if (arg.equals("-history")) {
                    options.maxHistory = Integer.parseInt(args[onArg+1]);
                    onArg += 2;
                } else if (arg.equals("-q") || arg.equals("--quiet")) {
                    options.quiet = true;
                    onArg++;
                } else {
                    break;
                }
            }
            
            // Make maxReadAhead the max possible, to make sure it's no more than max!
            options.maxReadAhead = 65535 - options.scanSize;
            
            String archiveName = args[onArg];
            onArg++;
            
            OutputStream out = ( archiveName.equals("-") ? (OutputStream)System.out : (OutputStream)new FileOutputStream(new File(archiveName)) );
            
            if (out == System.out) {
                // Redirect any output which would have gone to stdout, to stderr, so it doesn't get mixed up with the data output
                System.setOut(System.err);
            }
            
            // oldBinaryDiffMain(outputFilename, args);
            compressFilesHavingParsedArgs(args, onArg, out);
            
        }
        
    }

    public static  void compressFilesHavingParsedArgs(String[] args, int argsAlreadyParsed, OutputStream out) throws IOException {
        
        Outputter output = new Outputter(options,out);
        StatsRepetitionFinder repetitionFinder = new StatsRepetitionFinder(); // options,in,output);
        
        Vector inputStreams = new Vector();
        for (int i=argsAlreadyParsed;i<args.length;i++) {
            File file = new File(args[i]);
            repetitionFinder.addInputStreamOrStreamsForFile(file,inputStreams,output);
        }
        InputStream in = new SequenceInputStream(inputStreams.elements());
        
        options.printOptionsToOutput(output);
        output.output_line("");
        
        // new RepetitionFinder().run(options, in, output); // ,out);
        repetitionFinder.run(options, in, output); // ,out);
        
        System.err.println();
        System.err.println(repetitionFinder.resultString());
        
    }
    
    class BinaryDiffer {
        
        // long totalBytes = 0;
        
    }
    
}
