<?php

class DBOperation{

	private $db;

	function DBOperation () {
		/*$dbhost = "localhost:3306";
		$dbuser = "root";
		$dbpass = "Wangym0124";
		$dbname = "test";*/

		$dbhost = "localhost";
		$dbuser = "cs143";
		$dbpass = "";
		$dbname = "CS143";
		$my_db = new mysqli($dbhost, $dbuser, $dbpass, $dbname);
		if($my_db->connect_errno > 0){
    		die('Unable to connect to database [' . $my_db->connect_error . ']');
		}

		$this->db = $my_db;
	}	

	function addActor($actor){
		//get parameters
		$last = $actor['last'];
		$first = $actor['first'];
		$sex = $actor['sex'];
		$dob = $actor['dob'];
		$dod = $actor['dod'];

		//insert comments and get comment id
        if ( $stmt = $this->db->prepare("SELECT id FROM MaxPersonID") ){
            #$stmt->bind_param('i', $id);
            $stmt->execute();
            $stmt->bind_result($id);
            $stmt->fetch();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
        $stmt->close();
		$newID = $id + 1;

		//update newID
		if ( $stmt = $this->db->prepare("UPDATE MaxPersonID SET id=?") ){
            $stmt->bind_param('i', $newID);
            $stmt->execute();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
        $stmt->close();

        if($dod == ''){
            $stmt = $this->db->prepare("INSERT INTO Actor(id, last, first, sex, dob, dod) Values (?, ?, ?, ?, ?, NULL)");
            $stmt->bind_param('issss', $newID, $last, $first, $sex, $dob);
        } else {
            $stmt = $this->db->prepare("INSERT INTO Actor(id, last, first, sex, dob, dod) Values (?, ?, ?, ?, ?, ?)");
            $stmt->bind_param('isssss', $newID, $last, $first, $sex, $dob, $dod);
        }
        $stmt->execute();
		$stmt->close();

	}

	function addDirector($director){
		//get parameters
		$last = $director['last'];
		$first = $director['first'];
		$dob = $director['dob'];
		$dod = $director['dod'];

		//insert comments and get comment id
        if ( $stmt = $this->db->prepare("SELECT id FROM MaxPersonID") ){
            #$stmt->bind_param('i', $id);
            $stmt->execute();
            $stmt->bind_result($id);
            $stmt->fetch();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
        $stmt->close();
		$newID = $id + 1;

		//update newID
		if ( $stmt = $this->db->prepare("UPDATE MaxPersonID SET id=?") ){
            $stmt->bind_param('i', $newID);
            $stmt->execute();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
        $stmt->close();

        if($dod == ''){
            $stmt = $this->db->prepare("INSERT INTO Director(id, last, first, dob, dod) Values (?, ?, ?, ?, NULL)");
            $stmt->bind_param('isss', $newID, $last, $first, $dob);
        } else {
            $stmt = $this->db->prepare("INSERT INTO Director(id, last, first, dob, dod) Values (?, ?, ?, ?, ?)");
            $stmt->bind_param('issss', $newID, $last, $first, $dob, $dod);
        }
        $stmt->execute();
        $stmt->close();
		
	}

	function addMovie($movie){
		//get parameters
		$title = $movie['title'];
		$year = $movie['year'];
		$rating = $movie['rating'];
		$company = $movie['company'];
		$genre = $movie['genre'];

		//insert comments and get comment id
        if ( $stmt = $this->db->prepare("SELECT id FROM MaxMovieID") ){
            $stmt->execute();
            $stmt->bind_result($id);
            $stmt->fetch();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
        $stmt->close();
		$newID = $id + 1;
		
		//update newID
		if ( $stmt = $this->db->prepare("UPDATE MaxMovieID SET id=?") ){
            $stmt->bind_param('i', $newID);
            $stmt->execute();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
        $stmt->close();
        
		if ( $stmt = $this->db->prepare("INSERT INTO Movie(id, title, year, rating, company) Values (?, ?, ?, ?, ?)") ){
            $stmt->bind_param('issss', $newID, $title, $year, $rating, $company);
            $stmt->execute();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
		$stmt->close();
		
		if ( $stmt = $this->db->prepare("INSERT INTO MovieGenre(mid, genre) Values (?, ?)") ){
            $stmt->bind_param('is', $newID, $genre);
            $stmt->execute();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
		$stmt->close();
		
	}

	function addReview($review){
		$title = $review['title'];
		$name = $review['name'];
		$rating = $review['rating'];
		$comment = $review['comment'];
		$datum = new DateTime();
		$time = $datum->format('Y-m-d H:i:s');

		if ( $stmt = $this->db->prepare("SELECT id FROM Movie WHERE title = ?") ){
			$stmt->bind_param('s', $title);
            $stmt->execute();
            $stmt->bind_result($id);
            $stmt->fetch();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
        $stmt->close();

		if ( $stmt = $this->db->prepare("INSERT INTO Review(name, time, mid, rating, comment) VALUES (?, ?, ?, ?, ?)") ){
			$stmt->bind_param('ssiis', $name, $time, $id, $rating, $comment);
            $stmt->execute();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
        $stmt->close();
	}

	function addActorMovie($data){
		$movie = $data['entityA'];
		$actor = $data['entityB'];
		$role = $data['role'];
		if ( $stmt = $this->db->prepare("SELECT id FROM Movie WHERE title = ?") ){
            $stmt->bind_param('s', $movie);
            $stmt->execute();
            $stmt->bind_result($mid);
            $stmt->fetch();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
        $stmt->close();

        $parse_name = explode(' ', $actor);
        if ( $stmt = $this->db->prepare("SELECT id FROM Actor WHERE first = ? AND last = ?") ){
            $stmt->bind_param('ss', $parse_name[0], $parse_name[1]);
            $stmt->execute();
            $stmt->bind_result($aid);
            $stmt->fetch();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
        $stmt->close();

		if ( $stmt = $this->db->prepare("INSERT INTO MovieActor(mid, aid, role) VALUES (?, ?, ?)") ){
			$stmt->bind_param('iis', $mid, $aid, $role);
            $stmt->execute();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
        $stmt->close();
	}

	function addDirectorMovie($data){
		$movie = $data['entityA'];
		$director = $data['entityB'];
		$role = $data['role'];
		if ( $stmt = $this->db->prepare("SELECT id FROM Movie WHERE title = ?") ){
            $stmt->bind_param('s', $movie);
            $stmt->execute();
            $stmt->bind_result($mid);
            $stmt->fetch();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
        $stmt->close();

        $parse_name = explode(' ', $director);
        if ( $stmt = $this->db->prepare("SELECT id FROM Director WHERE first = ? AND last = ?") ){
            $stmt->bind_param('ss', $parse_name[0], $parse_name[1]);
            $stmt->execute();
            $stmt->bind_result($did);
            $stmt->fetch();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
        $stmt->close();
        
		if ( $stmt = $this->db->prepare("INSERT INTO MovieDirector(mid, did) VALUES (?, ?)") ){
			$stmt->bind_param('ii', $mid, $did);
            $stmt->execute();
        } else {
            echo "create_a_user: insert statement went wrong";
        }
        $stmt->close();
	}


	function searchActorsByID($aid){
		if($aid == ""){
			$error = "invalid actor ID";
			return $error;
		}else if ($stmt = $this->db->prepare("SELECT last, first, sex, dob, dod FROM Actor WHERE id = ?")){
            $stmt->bind_param('i', $aid);
            $stmt->execute();
            $stmt->bind_result($last, $first, $sex, $dob, $dod);
            $stmt->fetch();
            $actorInfo = array(
                "last" => $last,
                "first" => $first,
                "sex" => $sex,
                "dob" => $dob,
                'dod' => $dod,
            );
            $stmt->close();

            $stmt = $this->db->prepare("SELECT ma.role, m.title, ma.mid FROM MovieActor ma, Movie m WHERE ma.mid = m.id AND ma.aid = ?");
            $stmt->bind_param('i', $aid);
            $stmt->execute();
            $stmt->bind_result($role, $title, $mid);
            $amRelations = array();
            while ($row = $stmt->fetch()) {
                $var = array(
                    "role" => $role,
                    "title" => $title,
                    "mid" => $mid,
                );
                array_push($amRelations, $var);
            }

            $actors['actorInfo'] = $actorInfo;
            $actors['amRelations'] = $amRelations;
            return $actors;

        } else {
            echo "check_name_email_existed: query statement went wrong";
        }
        $stmt->close();
	}

	function searchActorsByName($name){
		$parse_name = explode(' ', $name);
		$statement = "SELECT id, first, last, dob FROM 
			Actor WHERE (last LIKE '%$parse_name[0]%' OR first LIKE '%$parse_name[0]%')";
		for($i = 1; $i < count($parse_name); $i++){
			$statement = $statement."AND (last LIKE '%$parse_name[$i]%' OR first LIKE '%$parse_name[$i]%')";
		}
		if ($stmt = $this->db->prepare($statement) ){
            
            $stmt->execute();
            $stmt->bind_result($id, $last, $first, $dob);
            $actors = array();
            while ($row = $stmt->fetch()) {
                $var = array(
                	"id" => $id,
                    "last" => $last,
                    "first" => $first,
                    "dob" => $dob,
                );
                array_push($actors, $var);
            }
            return $actors;

        } else {
            echo "check_name_email_existed: query statement went wrong";
        }
        $stmt->close();
	}

	function searchMoviesByID($mid){
		if($mid == ""){
			$error = "Invalid Movie ID";
			return $error;
		}else if ($stmt = $this->db->prepare("SELECT title, year, rating, company FROM Movie WHERE id = ?") ){
            $stmt->bind_param('i', $mid);
            $stmt->execute();
            $stmt->bind_result($title, $year, $rating, $company);
            $stmt->fetch();
            $movieInfo = array(
                "title" => $title,
                "year" => $year,
                "rating" => $rating,
                "company" => $company,
            );
            $stmt->close();
       

			$stmt = $this->db->prepare("SELECT d.last, d.first FROM MovieDirector md, Director d WHERE d.id = md.did AND md.mid = ?");
            $stmt->bind_param('i', $mid);
            $stmt->execute();
            $stmt->bind_result($last, $first);
            $stmt->fetch();
            $director = array(
                "last" => $last,
                "first" => $first,
            );
            $stmt->close();

            $stmt = $this->db->prepare("SELECT genre FROM MovieGenre WHERE mid = ?");
            $stmt->bind_param('i', $mid);
            $stmt->execute();
            $stmt->bind_result($genreinfo);
            $genre = array();
            while ($row = $stmt->fetch()) {
                array_push($genre, $genreinfo);
            };
            $stmt->close();

            $stmt = $this->db->prepare("SELECT a.first, a.last, ma.aid, ma.role FROM Actor a, MovieActor ma WHERE a.id = ma.aid AND ma.mid = ?");
            $stmt->bind_param('i', $mid);
            $stmt->execute();
            $stmt->bind_result($first, $last, $aid, $role);
            $amRelation = array();
            while ($row = $stmt->fetch()) {
                $var = array(
                    "first" => $first,
                    "last" => $last,
                    "aid" => $aid,
                    "role" => $role,
                );
                array_push($amRelation, $var);
            };
            $stmt->close();

            $stmt = $this->db->prepare("SELECT AVG(rating) FROM Review WHERE mid = ?");
            $stmt->bind_param('i', $mid);
            $stmt->execute();
            $stmt->bind_result($ave_rating);
            $stmt->fetch();
            $stmt->close();

            $stmt = $this->db->prepare("SELECT time, name, rating, comment FROM Review WHERE mid = ? ORDER BY time DESC");
            $stmt->bind_param('i', $mid);
            $stmt->execute();
            $stmt->bind_result($time, $name, $rating, $comment);
            $comments = array();
            while ($row = $stmt->fetch()) {
                $var = array(
                    "time" => $time,
                    "name" => $name,
                    "rating" => $rating,
                    "comment" => $commemt,
                );
                array_push($comments, $var);
            };
			$stmt->close();
			
			$movie['movieInfo'] = $movieInfo;
			$movie['director'] = $director;
			$movie['genres'] = $genre;
			$movie['amRelations'] = $amRelation;
			$movie['aveRating'] = $ave_rating;
			$movie['reviews'] = $comments;

			return $movie;
            #return array_merge($movie, $director, $genre, $actors, $ave_rating, $comments);

        } else {
            echo "check_name_email_existed: query statement went wrong";
        }
    }

	function searchMoviesByName($name){
		$parse_name = explode(' ', $name);
		$statement = "SELECT id, title, year FROM 
			Movie WHERE title LIKE '%$parse_name[0]%'";
		for($i = 1; $i < count($parse_name); $i++){
			$statement = $statement."AND title LIKE '%$parse_name[$i]%'";
		}
		if ($stmt = $this->db->prepare($statement) ){
            
            $stmt->execute();
            $stmt->bind_result($id, $title, $year);
            $movies = array();
            while ($row = $stmt->fetch()) {
                $var = array(
                	"id" => $id,
                    "title" => $title,
                    "year" => $year,
                );
                array_push($movies, $var);
            }
            return $movies;

        } else {
            echo "check_name_email_existed: query statement went wrong";
        }
        $stmt->close();
	}
}


?>