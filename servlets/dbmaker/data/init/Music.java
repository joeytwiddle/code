package dbmaker.data.init;

import java.lang.*;
import java.util.*;

public class Music {
	// List of all objects types
	Artist a;
	Album al;
	AlbumBy ab;
	TrackBy tb;
	Track t;
	AlbumTrack at;
	LoneTrack lt;
}

class Artist {
	String name;
}

class AlbumBy {
	Artist artist;
	Album album;
}

class TrackBy {
	Artist artist;
	LoneTrack track;
}

class Album {
	String title;
}

abstract class Track {
	String name;
	float length;
}

class AlbumTrack extends Track {
	Album album;
}

class LoneTrack extends Track {
}
