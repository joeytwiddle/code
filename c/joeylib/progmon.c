// ProgressMonitor

class ProgMon {

public:
	void (ProgMon::*nowthrough)(const float f);
	void (ProgMon::*end)();

	int unixterm_lastprnt;

	ProgMon() {
		unixterm_lastprnt=-1;
		nowthrough=&ProgMon::unixterm_nowthrough;
		end=&ProgMon::unixterm_end;
	}

	void unixterm_nowthrough(const float f) {
		#define UNIXTERMWID 70
		int toprnt=f*UNIXTERMWID;
		if (toprnt>unixterm_lastprnt) {
			printf("\r|");
			printf(Srep("=",toprnt-1));
			printf("#");
			printf(Srep("-",UNIXTERMWID-toprnt));
			printf("|");
			unixterm_lastprnt=toprnt;
		}
	}

	void unixterm_end() {
		printf("\r ");
		printf(Srep(" ",UNIXTERMWID));
		printf(" \r");
	}

};
