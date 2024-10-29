<?php
	global $pdo;
	
	try
	{
		$pdo = new PDO("mysql:host=localhost;dbname=xsladk07;port=/var/run/mysql/mysql.sock", 'xsladk07', 'm8jongoj');
	}
	catch(PDOException $e)
	{
		echo "Connection error: ".$e->getMessage();
		die();
	}
?>