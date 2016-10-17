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