package dbmaker.data.init;

import java.lang.*;
import java.util.*;

public class Generation {
	// List of all objects types
	Person p;
	PersonWithKnownAge pwka;
	Philosopher ph;
	Publication pu;
	Authorship au;
	ExampleInterface ei;
	ExampleImplementationA eia;
	ExampleImplementationB eib;
}

class Person {
	String firstName;
	String lastName;
}

class PersonWithKnownAge extends Person {
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
	Philosopher author;
}

interface /* or abstract class */ ExampleInterface {
}

class ExampleImplementationA implements ExampleInterface {
}

class ExampleImplementationB implements ExampleInterface {
}

