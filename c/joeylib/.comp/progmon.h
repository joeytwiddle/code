#ifndef progmon_H
  #define progmon_H


// ProgressMonitor

class ProgMon {

	#define UNIXTERMWID 70

public:
	void (ProgMon::*nowthrough)(const float f); // Exists
	void (ProgMon::*end)(); // Exists

	int unixterm_lastprnt; // Exists

	 ProgMon(); // Method


	void unixterm_nowthrough(const float f); // Method


	void unixterm_end(); // Method


};

#endif
