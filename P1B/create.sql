CREATE TABLE Movie(
	id		int	NOT NULL,
	title	varchar(100) NOT NULL,	
	year	int NOT NULL,
	rating	varchar(10),
	company	varchar(50),
	PRIMARY KEY (id),
	CHECK(year > 1000 AND year <= 2016)
);

CREATE TABLE Actor(
	id		int NOT NULL,
	last	varchar(20),
	first	varchar(20),
	sex		varchar(6),
	dob		date NOT NULL,
	dod		date,
	PRIMARY KEY (id),
	CHECK (sex = 'Female' OR sex = 'Male')
);

CREATE TABLE Director(
	id		int NOT NULL,
	last	varchar(20),
	first	varchar(20),
	dob		date NOT NULL,
	dod		date,
	PRIMARY KEY (id)	
);

CREATE TABLE MovieGenre(
	mid		int NOT NULL,
	genre 	varchar(20),
	FOREIGN KEY (mid) references Movie(id)
);

CREATE TABLE MovieDirector(
	mid  	int NOT NULL,
	did		int NOT NULL,
	FOREIGN KEY (mid) references Movie(id),
	FOREIGN KEY (did) references Director(id)
);

CREATE TABLE MovieActor(
	mid 	int NOT NULL,
	aid		int NOT NULL,
	role	varchar(50),
	FOREIGN KEY (mid) references Movie(id),
	FOREIGN KEY (aid) references Actor(id)
);

CREATE TABLE Review(
	name	varchar(20),
	time	timestamp,
	mid 	int NOT NULL,
	rating	int NOT NULL,
	comment varchar(500),
	FOREIGN KEY (mid) references Movie(id),
	CHECK (rating >= 0 AND rating <= 5)
);

CREATE TABLE MaxPersonID(
	id 		int NOT NULL
);

CREATE TABLE MaxMovieID(
	id 		int NOT NULL	
);




