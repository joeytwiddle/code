#ifndef progmon_C
  #define progmon_C

  #include <progmon.h>

// ProgressMonitor

// Starts of class ProgMon



	// Variable declared in .h file
	// Variable declared in .h file

	// Variable declared in .h file

	 ProgMon::ProgMon() {
		unixterm_lastprnt=-1;
		nowthrough=&ProgMon::unixterm_nowthrough;
		end=&ProgMon::unixterm_end;
	}

	void ProgMon::unixterm_nowthrough(const float f) {
		#define UNIXTERMWID 70
		int toprnt=f*UNIXTERMWID;
		if (toprnt>unixterm_lastprnt) {
			printf("\r|");
			printf(Srep("#",toprnt));
			printf(Srep("-",UNIXTERMWID-toprnt));
			printf("|");
			unixterm_lastprnt=toprnt;
		}
	}

	void ProgMon::unixterm_end() {
		printf("\r ");
		printf(Srep(" ",UNIXTERMWID));
		printf(" \r");
	}

// End class 


#endif
