<?php

    require_once "./Functions.php";

    $db_manager = new DBOperation();

    switch ($_SERVER['REQUEST_METHOD']) {

    case "GET":

        $entity = $_GET['entity'];
        $id = $_GET['id'];
        $name = $_GET['name'];

        if ($entity == 'actors') {
            if ($id != NULL) {
                $result = $db_manager->searchActorsByID($id);
            } else {
                $result = $db_manager->searchActorsByName($name);
            }
        } else if ($entity == 'movies') {

            if ($id != NULL) {
                $result = $db_manager->searchMoviesByID($id);
            } else {
                $result = $db_manager->searchMoviesByName($name);
            }
        }

    break;
    }
    $json = json_encode($result);
    echo $json;
?>