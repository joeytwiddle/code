// ProgressMonitor

class ProgMon {

	#define UNIXTERMWID 70

public:
	void (ProgMon::*nowthrough)(const float f);
	void (ProgMon::*end)();

	int unixterm_lastprnt;

	ProgMon() {
		unixterm_lastprnt=-1;
		nowthrough=&ProgMon::unixterm_nowthrough;
		end=&ProgMon::unixterm_end;
		// fprintf(stderr,"|");
		// fprintf(stderr,Srep("=",UNIXTERMWID));
		// fprintf(stderr,"|\n");
		// fprintf(stderr,">");
	}

	void unixterm_nowthrough(const float f) {
		int toprnt=f*UNIXTERMWID;
		if (toprnt>unixterm_lastprnt) {
			// fprintf(stderr,Srep(".",toprnt-unixterm_lastprnt));
			fprintf(stderr,"\r|");
			fprintf(stderr,Srep("=",toprnt));
			fprintf(stderr,"#");
			fprintf(stderr,Srep("-",UNIXTERMWID-1-toprnt));
			fprintf(stderr,"|");
			unixterm_lastprnt=toprnt;
		}
	}

	void unixterm_end() {
		// fprintf(stderr,"\r ");
		// fprintf(stderr,Srep(" ",UNIXTERMWID));
		// fprintf(stderr," \r");
		fprintf(stderr,"\n");
	}

};
