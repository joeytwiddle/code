#ifndef progmon_C
  #define progmon_C

  #include <progmon.h>

// ProgressMonitor

// Starts of class ProgMon


	#define UNIXTERMWID 70


	// Variable declared in .h file
	// Variable declared in .h file

	// Variable declared in .h file

	 ProgMon::ProgMon() {
		unixterm_lastprnt=-1;
		nowthrough=&ProgMon::unixterm_nowthrough;
		end=&ProgMon::unixterm_end;
		// fprintf(stderr,"|");
		// fprintf(stderr,Srep("=",UNIXTERMWID));
		// fprintf(stderr,"|\n");
		// fprintf(stderr,">");
	}

	void ProgMon::unixterm_nowthrough(const float f) {
		/*
		int toprnt=f*UNIXTERMWID;
		if (toprnt>unixterm_lastprnt) {
			fprintf(stderr,Srep(".",toprnt-unixterm_lastprnt));
			fprintf(stderr,"\r|");
			fprintf(stderr,Srep("=",toprnt));
			fprintf(stderr,"#");
			fprintf(stderr,Srep("-",UNIXTERMWID-1-toprnt));
			fprintf(stderr,"|");
			unixterm_lastprnt=toprnt;
		}
		*/
	}

	void ProgMon::unixterm_end() {
		/*
		// Finish it off and newline
		// fprintf(stderr,"\r|");
		// fprintf(stderr,Srep("=",UNIXTERMWID));
		// fprintf(stderr,"|\n");

		// Pretend it never happened
		fprintf(stderr,"\r ");
		fprintf(stderr,Srep(" ",UNIXTERMWID));
		fprintf(stderr," \r");
		*/
	}

// End class 


#endif
