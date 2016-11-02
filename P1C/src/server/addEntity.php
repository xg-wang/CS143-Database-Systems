
<?php

    require_once "./Functions.php";

    $db_manager = new DBOperation();

    switch ($_SERVER['REQUEST_METHOD']) {

    case "POST":
        $json = json_decode(file_get_contents('php://input'), true);
        $entity = $json["entity"];
        $data = $json["data"];
        $dateDob = explode('-', $data['dob']);
        $dateDod = explode('-', $data['dod']);
        
        if ($entity == 'actor' || $entity == 'director') {
            if($data['first'] == '' || $data['last'] == ''){
                $result = "Please enter a valid first and last name.";
            } else if (preg_match('/[^A-Za-z\s\'-]/', $data['first']) || preg_match('/[^A-Za-z\s\'-]/', $data['last'])){
                $result = "Invalid syntax for first and last name.";
            } else if ($entity == 'actor' && $data['sex'] == ''){
                $result = "Please choose the actor's gender.";
            } else if ($data['dob'] == '' || !checkdate($dateDob[1], $dateDob[2], $dateDob[0])){
                $result = "Please enter a valid date of birth";
            } else if ($data['dod'] != '' && !checkdate($dateDod[1], $dateDod[2], $dateDod[0])){
                $result = "Please enter a valid date of death";
            } else if ($entity == 'actor'){
                $db_manager->addActor($data);
                $result = "Successfully add a actor";
            } else {
                $db_manager->addDirector($data);
                $result = "Successfully add a director";
            }
        } else if ($entity == 'movie') {
            if($data['title'] == ''){
                $result = "Please enter a valid movie title.";
            } else if ($data['year'] == '' || $data['year'] > 2200){
                $result = "Please enter a valid movie year.";
            }else{
                $db_manager->addMovie($data);
                $result = "Successfully add a movie";
            }
        } else if ($entity == 'review'){
            if($data['title'] == ''){
                $result = "Please enter a valid movie title.";
            } else if ($data['rating'] == '' || $data['rating'] < 1 || $data['rating'] > 5){
                $result = "Please select a valid rating.";
            } else {
                $db_manager->addReview($data);
                $result = "Successfully add a comment";
            }
        }
    break;
    }
    $json = json_encode($result);
    echo $json;
?>