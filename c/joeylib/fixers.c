// Templates and inheritance sometimes need a nudge
// to get created!

// These just ensure certain templates functions are compiled
// into the shared library.

void ______fixers() {
	randbetween(V2d(),V2d());
	Map1d<float> m1f;
	m1f.draw();
	Map2d<float> m2f;
	Map2d<unsigned char> m2uc;
	m2uc.getrealregions();
	m2uc.getregionmap('c');
	RGBmp r;
	r.toRGBmp(&m2f);
	r.toRGBmp(&m2uc);
	r.thickcross(V2d(),0,1.0f,myRGB());
	Map2d<int> m2i;
	m2i.collectregions();
}
