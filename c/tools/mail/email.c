class Email {
public:
	List<String> header;
	List<String> content;
	Email() {
	}
	int parsefrom(List<String> src,int start) {
		int i=start;
		// Find end of header
		bool done=false;
		while (!done && i<=src.len) {
			if (Slen(src.num(i))>0) {
				header.add(src.num(i));
//printf("%s\n",src.num(i));
				i++;
			} else
				done=true;
		}
		i++;
//printf("end header\n");
		// Find next header
		done=false;
		while (!done && i<=src.len) {
			if (Sstarts(src.num(i),"From ")) {
//printf("Found start %s\n",src.num(i));
			  // Check its not a forwarded message
				Date th=Date(date());
				String test=grabrestofline("Date: ",&src,i);
				Date that=Date(test);
//printf("Testing %s against %s\nresult %i\n",th.toString(),that.toString(),that>=th);
				if (that>=th)
					done=true;
				else
					i++;
//printf("%i Got date %s\n",done,that.toString());
			} else {
				content.add(src.num(i));
// printf("Content %s\n",src.num(i));
    		i++;
			}
		}
		return i;
	}
	int linestarting(String s,List<String> *ls,int x) {
		for (int i=x;i<=ls->len;i++) {
			if (Sstarts(ls->num(i),s)>0)
				return i;
		}
		return -1;
	}
	String grabrestofline(String s,List<String> *ls,int x) {
//		printf("Finding line starting >%s<\n",s);
		int i=linestarting(s,ls,x);
//		printf("Got: %s\n",ls->num(i));
		if (i>0)
			return Sfrom(ls->num(i),Slen(s)+1);
		else
			return "";
	}
	String find(String s) {
		return grabrestofline(s,&header,1);
	}
	String from() {
		return find("From: ");
	}
	String to() {
		return find("To: ");
	}
	String subject() {
		return find("Subject: ");
	}
	String date() {
		return find("Date: ");
	}
	List<String> whole() {
		List<String> w;
		w.add(header);
		w.add("");
		w.add(content);
		return w;
	}
	void freedom() {
		header.freedom();
		content.freedom();
	}
};