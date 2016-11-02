<?php

    require_once "./Functions.php";
    $db_manager = new DBOperation();

    switch ($_SERVER['REQUEST_METHOD']) {

    case "POST":
        $json = json_decode(file_get_contents('php://input'), true);

        $relation = $json["entity"];
        $data = $json["data"];

        if ($relation == 'actorMovie') {
            
            if($data['title'] == ''){
                $result = "You must type a movie.";
            } else if($data['name'] == ''){
                $result = "You must type a actor.";
            }else{
                $result = $db_manager->addActorMovie($data);
                if($result == 1){
                    $result = "movie does not exits";
                }else if($result == 2){
                    $result = "actor does not exits";
                }else {
                    $result = "Successfully add a movie-actor relation.";
                }
            }
        }

        if ($relation == 'directorMovie') {
            if($data['title'] == ''){
                $result = "You must type a movie.";
            } else if($data['name'] == ''){
                $result = "You must type a director.";
            }else{
                $result = $db_manager->addDirectorMovie($data);
                if($result == 1){
                    $result = "movie does not exits";
                }else if($result == 2){
                    $result = "director does not exits";
                }else {
                    $result = "Successfully add a movie-director relation.";
                }
            }
        }
    break;
    }
    $json = json_encode($result);
    echo $json;
?>