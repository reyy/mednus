<?php
//
$type = htmlspecialchars($_GET["type"]);
//echo $GET['type2'];

// if(!($type == 'model' || $type == 'pdf' || $type == 'video'))
// 	die("Invalid Request.");

//$dbconn = pg_connect("host=localhost port=5432 dbname=anatomy user=anatomy password=foo connect_timeout=10");

$filename = "craniofacial.ply";
$filepath = "resource_files/mednus/lesson1/".$type."/".$filename;
echo $filepath;
// make sure it's a file before doing anything!
if(file_exists($filepath)) {

	//$dbconn = pg_connect("host=localhost port=5432 dbname=anatomy user=anatomy password=foo connect_timeout=10");


	// required for IE
	if(ini_get('zlib.output_compression')) { ini_set('zlib.output_compression', 'Off');	}

	// get the file mime type using the file extension
	switch(strtolower(substr(strrchr($filename, '.'), 1))) {
		case 'pdf': $mime = 'application/pdf'; break;
		case 'zip': $mime = 'application/zip'; break;
		case 'jpeg':
		case 'jpg': $mime = 'image/jpg'; break;
		default: $mime = 'application/force-download';
	}
	header('Pragma: public'); 	// required
	header('Expires: 0');		// no cache
	header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
	header('Last-Modified: '.gmdate ('D, d M Y H:i:s', filemtime ($filepath)).' GMT');
	header('Cache-Control: private',false);
	header('Content-Type: '.$mime);
	header('Content-Disposition: attachment; filename="'.basename($filepath).'"');
	header('Content-Transfer-Encoding: binary');
	header('Content-Length: '.filesize($filepath));	// provide file size
	header('Connection: close');
	readfile($filepath);		// push it out
	exit();

}
else {
die("Invalid Request.2");
}
?>
