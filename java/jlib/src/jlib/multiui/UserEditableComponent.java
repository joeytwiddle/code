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
import nuju.*;

public interface UserEditableComponent {

	public void setValue(String value);

	/* GetValue not implemented here so it can be of any type depending on the component. **/

}
