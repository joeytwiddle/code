// Decompiled by Jad v1.5.8d. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.geocities.com/kpdus/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   Fire1.java

import java.awt.image.*;
import java.util.*;

class Fire1Producer extends Thread
    implements ImageProducer
{

    public Fire1Producer(int i, int j)
    {
        seed = 1234L;
        Props = new Hashtable();
        Consumers = new Vector();
        seed = 0x76be9ffL;
        Width = i;
        Height = j;
        Red = new byte[128];
        Blue = new byte[128];
        Green = new byte[128];
        for(int k = 0; k < 64; k++)
        {
            Red[k] = (byte)((k * 255) / 64);
            Blue[k] = 0;
            Green[k] = 0;
        }

        for(int l = 0; l < 64; l++)
        {
            Red[l + 64] = -1;
            Blue[l + 64] = (byte)((l * 255) / 64);
            Green[l + 64] = 0;
        }

        DCModel = new IndexColorModel(8, 128, Red, Blue, Green);
        Hash1 = new byte[128];
        Low_Rand = new int[128];
        for(int i1 = 0; i1 < 128; i1++)
            Hash1[i1] = (byte)i1;

        for(int j1 = 0; j1 < 500; j1++)
        {
            byte byte0 = Hash1[(int)(My_Rand() >> 11) & 0x7f];
            byte byte1 = Hash1[(int)(My_Rand() >> 12) & 0x7f];
            byte byte2 = Hash1[byte0];
            Hash1[byte0] = Hash1[byte1];
            Hash1[byte1] = byte2;
        }

        for(int k1 = 0; k1 < 128; k1++)
        {
            int i2 = (int)(My_Rand() >> 12 & 15L);
            i2 %= 3;
            Low_Rand[k1] = i2 - 1;
        }

        Tiny_Seed = 0;
        Video = new int[Width * Height];
        for(int l1 = 0; l1 < Width * Height; l1++)
            Video[l1] = Low_Rand[Tiny_Seed = Hash1[Tiny_Seed]] + 15;

    }

    public long My_Rand()
    {
        seed *= 0x10003L;
        return seed;
    }

    long draw_fire(long l)
    {
        seed = l;
        int k1 = Width;
        for(int i1 = Height - 2; i1 >= 0; i1--)
        {
            for(int j = Width - 1; j >= 0; j--)
            {
                int i = Video[k1 + j];
                if(i > 0)
                {
                    Tiny_Seed = Hash1[Tiny_Seed];
                    int j1 = Low_Rand[Tiny_Seed];
                    if(j1 != 0)
                    {
                        if((j += j1) < 0)
                            j = 0;
                        else
                        if(j >= Width)
                            j = Width - 1;
                        if((i -= 2) < 0)
                            i = 0;
                    }
                }
                Video[(k1 - Width) + j] = i;
            }

            k1 += Width;
        }

        for(int k = 0; k < 2 * Width; k++)
            Video[(Height - 2) * Width + k] = 127 - (int)(My_Rand() >> 7 & 63L);

        return seed;
    }

    void Do_Fire()
    {
        seed = draw_fire(seed);
        Produce(Consumers);
    }

    void Produce(Vector vector)
    {
        ImageConsumer imageconsumer;
        for(Enumeration enumeration = vector.elements(); enumeration.hasMoreElements(); imageconsumer.imageComplete(2))
        {
            imageconsumer = (ImageConsumer)enumeration.nextElement();
            imageconsumer.setPixels(0, 0, Width, Height, DCModel, Video, 0, Width);
        }

    }

    public synchronized void addConsumer(ImageConsumer imageconsumer)
    {
        Consumers.addElement(imageconsumer);
        imageconsumer.setDimensions(Width, Height);
        imageconsumer.setProperties(Props);
        imageconsumer.setColorModel(DCModel);
        imageconsumer.setHints(14);
    }

    public synchronized boolean isConsumer(ImageConsumer imageconsumer)
    {
        return Consumers.contains(imageconsumer);
    }

    public synchronized void removeConsumer(ImageConsumer imageconsumer)
    {
        Consumers.removeElement(imageconsumer);
    }

    public void startProduction(ImageConsumer imageconsumer)
    {
        Consumers.addElement(imageconsumer);
        imageconsumer.setDimensions(Width, Height);
        imageconsumer.setProperties(Props);
        imageconsumer.setColorModel(DCModel);
        imageconsumer.setHints(14);
        Produce(Consumers);
    }

    public void requestTopDownLeftRightResend(ImageConsumer imageconsumer)
    {
        Produce(Consumers);
    }

    Vector Consumers;
    IndexColorModel DCModel;
    Hashtable Props;
    public int Video[];
    public int Width;
    public int Height;
    byte Red[];
    byte Blue[];
    byte Green[];
    public long seed;
    byte Tiny_Seed;
    byte Hash1[];
    int Low_Rand[];
}
