char Sfromhex(String s) {
  char c;
  c=( s[0] >= 'A' ? ((s[0] & 0xdf) - 'A')+10 : (s[0] - '0')) * 16;
  c+=( s[1] >= 'A' ? ((s[1] & 0xdf) - 'A')+10 : (s[1] - '0'));
  return c;
}


String Sunescape(String p) {
  String s=Snew(Slen(p));
  for (int i=0;i<Slen(p);i++)
    if (p[i]=='+')
      s[i]=' ';
    else
      s[i]=p[i];
  String n=Snew(Slen(s));
  int j=0;
  for (int i=0;i<Slen(s);i++) {
    if (s[i]=='%') {
      n[j]=Sfromhex(&s[i+1]);
      j++;
      i++; i++;
    } else {
      n[j]=s[i];
      j++;
    }
  }
  return n;
}


List<String> Ssplitkeeping(String s,String yes) {
  List<String> ws;
  String t="";
  for (int i=1;i<=Slen(s);i++) {
    String m=Smid(s,i);
    if (Sinstr(yes,m)>0) {
      t=Sconc(t,m);
//      free(t);
    } else {
      if (Slen(t)>0) {
        ws.add(Snew(t));
        free(t);
        t="";
      }
    }
    free(m);
  }
  if (Slen(t)>0)
    ws.add(Snew(t));
	if (Slen(t)>0)
	  free(t);
  return ws;
}

List<String> Ssplitaround(String s,String split) {
  List<String> ws;
  bool done=false;
  while (!done) {
    int i=Sinstr(s,split);
    if (i<1) {
      if (Slen(s)>0)
        ws.add(s);
      done=true;
    } else {
      ws.add(Sbefore(s,split));
      s=Safter(s,split);
    }
  }
  return ws;
}

List<String> Ssplittowords(List<String> ls) {
	List<String> ws;
	String alpha=Salphabet();
	for (int i=1;i<=ls.len;i++) {
		// ws.add(Ssplit(ls.num(i),Salphabet()));
		// Due to inefficient memory management, becomes
		List<String> a=Ssplitkeeping(ls.num(i),alpha);
		ws.add(a);
		a.freedom();
	}
	free(alpha);
	return ws;
}