<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);

function validateLogin($token) 
{
	$url = "https://ivle.nus.edu.sg/api/Lapi.svc/Validate?"."APIKey=lAY3TAAcAGYcokEEqKNCt&"."Token=".$token;

	$result = curl_post($url);
	echo $result;
	$resultArray = json_decode($result,true);
	echo $resultArray["Success"];
    if($resultArray["Success"])
        return true;
    else
        return false;
}

function curl_post($url, $post = NULL) 
{ 
    $defaults = array( 
        //CURLOPT_POST => 0, 
        CURLOPT_HEADER => 0, 
        CURLOPT_URL => $url, 
        CURLOPT_FRESH_CONNECT => 1, 
        CURLOPT_RETURNTRANSFER => 1, 
        CURLOPT_FORBID_REUSE => 1, 
        // CURLOPT_TIMEOUT => 4, 
        // CURLOPT_POSTFIELDS => http_build_query($post),
        CURLOPT_SSL_VERIFYPEER => true
    ); 

    $ch = curl_init(); 
    curl_setopt_array($ch, ($defaults)); 

    if(!$result = curl_exec($ch)) 
        trigger_error(curl_error($ch)); 
    
    curl_close($ch); 

    return $result; 
} 

?>