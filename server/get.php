<?php

//Check isset
if(!isset($_GET["type"]) || !isset($_GET["lessonid"]) || !isset($_GET["fileid"]))
	die("Invalid Paramters");

$lessonid = $_GET["lessonid"];
$fileid = $_GET["fileid"];
$type = $_GET["type"];

//Valid Type Check
if(!($type == 'model' || $type == 'pdf' || $type == 'video') ||
	!(is_int($fileid) && is_int($lessionid)))
	die("Invalid Request.");



include("settings.php");
include("utility.php");
//include("auth.php");

//Do Query
$result = pg_prepare($dbconn, "get_pdf" ,'SELECT get_pdf($1,$2,$3);');
$result = pg_execute($dbconn, "get_pdf", array('pdf',$lessonid, $fileid));

$rows = pg_num_rows($result);
$cols = pg_num_fields($result);

if($rows == 0)
	die("Invalid File Error.");
else if($rows > 1 || $cols > 1)
	die("Database Error. Please contact Admin.");
else
	$filename = pg_fetch_result($result, 0);

//echo $filename;

//Cleanup
pg_free_result($result);



//Download File
$filepath = "resource_files/mednus/".$lessonid."/".$type."/".$filename;
//$filepath = "resource_files/mednus/lesson1/models/craniofacial.ply";
//echo $filepath;

// make sure it's a file before doing anything!
if(file_exists($filepath))
	downloadFile($filepath);
else
	die("Lost File Error. Please contact Admin.");

?>
