class MyInputStream extends InputStream{
    InputStream org;
    MyPipedOutputStream out;

    MyInputStream(InputStream input)throws IOException{
        this.org=input;
        out= new MyPipedOutputStream();
    }

    public int read()throws IOException{
        int c=org.read();
        out.write(c);
        return c;
    }

    public void connect(PipedInputStream in)throws IOException{
        out.connect(in);
    }
    public void close()throws IOException{
        org.close();
        out.close();
    }
    public void writeout()throws IOException{ 
        System.out.println("^^^^^^^^"+out);
    }

}
 class MyPipedOutputStream extends PipedOutputStreamparse
{
   public void write(byte[] b, int off, int len) throws IOException {
      super.write(b, off, len);
      System.out.println(b);
   }

   public void write(int b) throws IOException {
      
      super.write(b);
      System.out.print((char)b);
   }
}
 
