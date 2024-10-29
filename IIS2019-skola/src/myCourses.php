<?php
    require_once 'functions/loader.php';

    //checking if logged user
    if(!isset($_SESSION['ISSET']))
    {
?>
        <h1>Unauthorized access!</h1>
<?php
        exit();
    }
    
    /* 
        Data preparation
    */
    //init some cariables
    $studentCourse = false;
    $lectorCourses = false;
    $guarantorCourses = false;

    //student courses
    if(isStudent())
    {
        $studentCourses = $pdo->prepare('SELECT * FROM registred R JOIN courses C ON R.courseId = C.courseId WHERE R.userId = :userId');
        $studentCourses->bindParam(':userId', $_SESSION['userId']);
        $studentCourses->execute();
    }

    //lector courses
    if(isLector())
    {
        $lectorCourses = $pdo->prepare('SELECT C.type, C.courseId, C.name FROM lectors L JOIN courses C ON C.courseId = L.courseId WHERE L.userId = :userId');
        $lectorCourses->bindParam(':userId', $_SESSION['userId']);
        $lectorCourses->execute();
    }

    //guarantor courses
    if(isGuarantor())
    {
        $guarantorCourses = $pdo->prepare('SELECT * FROM courses WHERE guarantor = :guarantor');
        $guarantorCourses->bindParam(':guarantor', $_SESSION['userId']);
        $guarantorCourses->execute();
    }

    require_once 'modules/header.php';
?>

<!-- Student block -->
<?php
    if(isStudent())
    {
?>
        <h2>Student Courses</h2>
        <table>
            <tr>
                <th>Name</th>
                <th>Type</th>
                <th>Score</th>
            </tr>
<?php
        foreach($studentCourses as $row)
        {
?>
            <tr>
                <th><a href=<?php echo 'courseDetail.php?id='.$row['courseId']?>><?php echo $row['name']?></th>
                <th><?php echo $row['type']?></th>
                <th><?php echo $row['score']?></th>
            </tr>
<?php    
        }
?>
        </table>
<?php
    }
?>

<!-- Lector block -->
<?php
    if(isLector())
    {
?>
        <h2>Lector Courses</h2>
        <table>
            <tr>
                <th>Name</th>
                <th>Type</th>
            </tr>
<?php
        foreach($lectorCourses as $row)
        {
?>
            <tr>
                <th><a href=<?php echo 'courseDetail.php?id='.$row['courseId']?>><?php echo $row['name']?></th>
                <th><?php echo $row['type'] ?></th>
            </tr>
<?php    
        }
?>
        </table>
<?php
    }
?>

<!-- Guarantor block -->
<?php
    if(isGuarantor())
    {
?>
        <h2>Guarant Courses</h2>
        <table>
            <tr>
                <th>Name</th>
                <th>Type</th>
            </tr>
<?php
        foreach($guarantorCourses as $row)
        {
?>
            <tr>
                <th><a href=<?php echo 'courseDetail.php?id='.$row['courseId']?>><?php echo $row['name']?></th>
                <th><?php echo $row['type'] ?></th>
            </tr>
<?php    
        }
?>
        </table>
<?php
    }
?>

<?php
    require_once 'modules/footer.php';
?>