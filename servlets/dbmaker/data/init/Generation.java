package dbmaker.data.init;

import java.lang.*;
import java.util.*;

public class Generation {
	Person p; PersonWithKnownAge pwka; Philosopher ph; Publication pu; Authorship au;
}

class Person {
	String firstName;
	String lastName;
}

class PersonWithKnownAge {
	Date dateOfBirth;
	Date dateOfDeath;
}

class Philosopher extends PersonWithKnownAge {
}

class Publication {
	String title;
	Date date;
}

/* This allows multiple authors of one document */
class Authorship {
	Publication publication;
	Person author;
}

