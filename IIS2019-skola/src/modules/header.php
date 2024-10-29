<!DOCTYPE html>
	<html>
		<head>
			<meta charset="UTF-8">
			<title>IIS - skola</title>
			<link rel="stylesheet" type="text/css" href="styles/style.css">
		</head>

	<body>

<?php
    //If logged, print user name
    if(isset($_SESSION['ISSET']))
    {
        echo "user: ".$_SESSION['name'];
    }
?>
    <!-- Button to return to overview -->
    <form action="home.php" method="post">
            <input type="submit" value="Overwiev" />
    </form>
<?php
	//if logged user, generate menu
    if(isset($_SESSION['ISSET']))
    {
?> 
        <!-- Button for myCourses -->
        <form action="myCourses.php" method="post">
            <input type="submit" value="My Courses" />
        </form>
        <!-- Button for shedule -->
        <form action="shedule.php" method="post">
            <input type="submit" value="Shedule" />
        </form>
        <!-- Button for logout -->
        <form action="logout.php" method="post">
            <input type="submit" value="Logout" />
        </form>
<?php
    }
    //else generate menu for guest
    else
    { 
?>
        <!-- Button for login -->
        <form action="login.php" method="post">
            <input type="submit" value="Login" />
        </form>
        <!-- Button for registration -->
        <form action="register.php" method="post">
            <input type="submit" value="Register" />
        </form>
<?php
    }
?>