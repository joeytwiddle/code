package org.neuralyte.common;

/** joey Nov 3, 2004 4:36:09 PM */
public class Assertion {
    public static void assertThat(boolean condition) {
        if (!condition) {
            try {
                throw new Exception("Assertion failed");
            } catch (Exception e) {
            	org.neuralyte.Logger.error(e);
            }
            // Could throw an Error or even System.exit()!
        }
    }
}
