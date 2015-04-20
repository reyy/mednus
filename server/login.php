<?php

// error_reporting(E_ALL);
// ini_set('display_errors', 1);

//Check isset
if(!isset($_GET["token"]) || !isset($_GET["nusnet_id"]) || !isset($_GET["domain"]))
	die("Invalid Paramters");

$token = $_GET["token"];
$nusnet = $_GET["nusnet_id"];
$domain = $_GET["domain"];

//CURL NOT ENABLE ON THIS SERVER, CANNOT DO AUTH CHECK :(
//Validate Token with IVLE first
/*
include("validate.php");
if(!validateLogin($token))
	die("Invalid Token");
*/

//Verify with DB
include("settings.php");

//nusnet_id, domain
$result = pg_prepare($dbconn, "check_permission" ,'SELECT check_permission($1,$2);');
$result = pg_execute($dbconn, "check_permission", array($nusnet, $domain));

if(pg_num_rows($result) == 1)
{
	echo pg_fetch_result($result, 0);
	return;
}
else
	die("Not a valid user.");




?>