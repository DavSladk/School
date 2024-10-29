<?php
    require_once 'functions/loader.php';

    
    if( isset($_SESSION['ISSET']) && ( isset($_POST['addLector']) || isset($_POST['removeLector']) ) )
    {
        if(isset($_POST['addLector']))
        {
            $stmt = $pdo->prepare('INSERT INTO lectors (userId, courseId) VALUES(:userId, :courseId)');
            $stmt->bindParam(':userId', $_POST['userId']);
            $stmt->bindParam(':courseId', $_POST['courseId']);
            $stmt->execute();
        }
        elseif(isset($_POST['removeLector']))
        {
            $stmt = $pdo->prepare('DELETE FROM lectors WHERE userId=:userId AND courseId=:courseId');
            $stmt->bindParam(':userId', $_POST['userId']);
            $stmt->bindParam(':courseId', $_POST['courseId']);
            $stmt->execute();
        }
    }
    else
    {
?>
<h1>Unauthorized access.</h1>
<?php
    exit();
    }

    header('Location: courseDetail.php?id='.$_POST['courseId']);
?>