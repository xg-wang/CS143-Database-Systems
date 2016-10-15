-- Show the results of actors' first and last name who act in the movie 'Die Another Day'.
SELECT CONCAT(a.first,' ', a.last)
FROM Movie m, Actor a, MovieActor ma
WHERE m.id = ma.mid AND a.id = ma.aid AND m.title = 'Die Another Day';

-- Show the count of all actors who acted in multiple movies.
SELECT COUNT(*)
FROM (SELECT a.id 
	FROM Actor a, MovieActor ma
	WHERE a.id = ma.aid
	GROUP BY a.id
	HAVING COUNT(mid) > 1) A;

-- Show the movies name where a person acted as both director and actor.
SELECT m.title
FROM Movie m, MovieDirector md, MovieActor ma
WHERE md.mid = ma.mid AND md.did = ma.aid AND md.mid = m.id;