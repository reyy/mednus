<?php

//TODO: Check user auth

include("settings.php");

//Do Query
$result = pg_prepare($dbconn, "get_lessonlist" ,'SELECT * from lessonlist;');
$result = pg_execute($dbconn, "get_lessonlist", array());

// $resultArray = pg_fetch_all($result);
// echo json_encode($resultArray);
$myarray = array();
while ($row = pg_fetch_assoc($result)) {
  $myarray[] = $row;
}

echo json_encode($myarray);


?>