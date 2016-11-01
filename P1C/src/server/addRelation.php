<?php

    require_once "./Functions.php";
    $db_manager = new DBOperation();

    switch ($_SERVER['REQUEST_METHOD']) {

    case "POST":
        $json = json_decode(file_get_contents('php://input'), true);

        $relation = $json["relation"];
        $data = $json['data'];

        if ($relation == 'actorMovie' && $data['entityA'] == '') {
            $result = "You must type a movie.";
        } else if($data['entityB'] == ''){
            $result = "You must type a actor.";
        }else{
            $db_manager->addActorMovie($data);
            $result = "Successfully add a movie-actor relation.";
        }

        if ($relation == 'directorMovie' && $data['entityA'] == '') {
            $result = "You must type a movie.";
        } else if($data['entityB'] == ''){
            $result = "You must type a director.";
        }else{
            $db_manager->addDirectorMovie($data);
            $result = "Successfully add a movie-director relation.";
        }
    break;
    }
    $json = json_encode($result);
    echo $json;
?>