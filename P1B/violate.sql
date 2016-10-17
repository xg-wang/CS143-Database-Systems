INSERT INTO Actor VALUES(1,"Wang","Xingan","Male",19950101,NULL)
-- ERROR 1062 (23000): Duplicate entry '1' for key 'PRIMARY'

INSERT INTO Movie VALUES(4733,"Database Rocks",2016,"PG-13","UCLA")
-- ERROR 1062 (23000): Duplicate entry '4733' for key 'PRIMARY'

INSERT INTO Director VALUES(37146,"Professor","X",19521112,\N)
-- ERROR 1062 (23000): Duplicate entry '37146' for key 'PRIMARY'

INSERT INTO Movie VALUES(4733,"Database Rocks",2017,"PG-13","UCLA")
-- CHECK(year > 1000 AND year <= 2016, but here year=2017

INSERT INTO Actor VALUES(68626,"Wang","Xingan","God",19950101,NULL)
--CHECK (sex = 'Female' OR sex = 'Male') but here it is neither

INSERT INTO Review VALUES("xgwang","2008-01-19 03:14:07",1639,6,"Awesome Movie");
-- CHECK (rating >= 0 AND rating <= 5) but here it is 6

INSERT INTO MovieGenre VALUES (4800, 'Thrill');
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails ('CS143'.'MovieGenre', CONSTRAINT 'MovieGenre_ibfk_1' FOREIGN KEY ('mid') REFERENCES 'Movie' ('id'))
-- Reference constraint error: Movie id(4800) does not exist in the Movie table

INSERT INTO MovieDirector VALUES (4800, 68626);
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails ('CS143'.'MovieDirector', CONSTRAINT 'MovieDirector_ibfk_1' FOREIGN KEY ('mid') REFERENCES 'Movie' ('id'))
-- Reference constraint error: Movie id(4800) does not exist in the Movie table

INSERT INTO MovieDirector VALUES (4734, 69000);
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails ('CS143'.'MovieDirector', CONSTRAINT 'MovieDirector_ibfk_2' FOREIGN KEY ('did') REFERENCES 'Director' ('id'))
-- Reference constraint error: Director id(69000) does not exist in the Director table

INSERT INTO MovieActor VALUES (4800, 66000, 'Judge');
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails ('CS143'.'MovieActor', CONSTRAINT 'MovieActor_ibfk_1' FOREIGN KEY ('mid') REFERENCES 'Movie' ('id'))
-- Reference constraint error: Movie id(4800) does not exist in the Movie table

INSERT INTO MovieActor VALUES (4734, 69000, 'Judge');
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails ('CS143'.'MovieActor', CONSTRAINT 'MovieActor_ibfk_2' FOREIGN KEY ('aid') REFERENCES 'Actor' ('id'))
-- Reference constraint error: Actor id(69000) does not exist in the Actor table

INSERT INTO Review VALUES ('David', '10-16-2016 23:17:16', 4800, 5, 'Very good!');
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails ('CS143'.'Review', CONSTRAINT 'MovieActor_ibfk_1' FOREIGN KEY ('mid') REFERENCES 'Movie' ('id'))
-- Reference constraint error: Movie id(4800) does not exist in the Movie table
