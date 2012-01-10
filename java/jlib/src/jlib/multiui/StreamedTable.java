package jlib.multiui;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import jlib.*;


/** Currently inefficient for large lists.  Static array re-creation! **/

public class StreamedTable extends Table {

	public StreamedTable(int w) {
		super(w,1);
	}

	public void set(int i,Component c) {
		tab[i][height-1]=c;
	}

	public void newRow() {
		height++;
		Component[][] tmp=new Component[width][height];
		for (int i=0;i<width;i++)
		for (int j=0;j<height-1;j++)
			tmp[i][j]=get(i,j);
		tab=tmp;
	}
  
}
