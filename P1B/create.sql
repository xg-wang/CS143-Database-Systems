CREATE TABLE Movie(
	id		int	NOT NULL,
	title	varchar(100) NOT NULL,	
	year	int NOT NULL,
	rating	varchar(10),
	company	varchar(50),
	PRIMARY KEY (id),
	CHECK(year > 1000 AND year <= 2016)
)ENGINE=INNODB;

CREATE TABLE Actor(
	id		int NOT NULL,
	last	varchar(20),
	first	varchar(20),
	sex		varchar(6),
	dob		date NOT NULL,
	dod		date,
	PRIMARY KEY (id),
	CHECK (sex = 'Female' OR sex = 'Male')
)ENGINE=INNODB;

CREATE TABLE Director(
	id		int NOT NULL,
	last	varchar(20),
	first	varchar(20),
	dob		date NOT NULL,
	dod		date,
	PRIMARY KEY (id)	
)ENGINE=INNODB;

CREATE TABLE MovieGenre(
	mid		int NOT NULL,
	genre 	varchar(20),
	FOREIGN KEY (mid) references Movie(id)
)ENGINE=INNODB;

CREATE TABLE MovieDirector(
	mid  	int NOT NULL,
	did		int NOT NULL,
	FOREIGN KEY (mid) references Movie(id),
	FOREIGN KEY (did) references Director(id)
)ENGINE=INNODB;

CREATE TABLE MovieActor(
	mid 	int NOT NULL,
	aid		int NOT NULL,
	role	varchar(50),
	FOREIGN KEY (mid) references Movie(id),
	FOREIGN KEY (aid) references Actor(id)
)ENGINE=INNODB;

CREATE TABLE Review(
	name	varchar(20),
	time	timestamp,
	mid 	int NOT NULL,
	rating	int NOT NULL,
	comment varchar(500),
	FOREIGN KEY (mid) references Movie(id),
	CHECK (rating >= 0 AND rating <= 5)
)ENGINE=INNODB;

CREATE TABLE MaxPersonID(
	id 		int NOT NULL
)ENGINE=INNODB;

CREATE TABLE MaxMovieID(
	id 		int NOT NULL	
)ENGINE=INNODB;




