package org.common.lib;

import java.io.IOException;
import java.io.InputStream;

import org.neuralyte.Logger;
import org.neuralyte.common.io.StreamUtils;

public class Streamer extends Thread {

    InputStream in;

    public  String result;

    public Streamer(InputStream in) {
        this.in = in;
    }

    public void run() {
        try {
            result = StreamUtils.streamStringFrom(in);
        } catch (IOException e) {
            Logger.error(e);
            result = null;
        }
    }

}
