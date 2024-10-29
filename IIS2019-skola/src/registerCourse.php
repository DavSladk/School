<?php
    require_once 'functions/loader.php';

    
    if( isset($_SESSION['ISSET']) && ( isset($_POST['register']) || isset($_POST['unregister']) ) )
    {
        if(isset($_POST['register']))
        {
            $stmt = $pdo->prepare('INSERT INTO registred (userId, courseId) VALUES(:userId, :courseId)');
            $stmt->bindParam(':userId', $_POST['userId']);
            $stmt->bindParam(':courseId', $_POST['courseId']);
            $stmt->execute();
        }
        elseif(isset($_POST['unregister']))
        {
            $stmt = $pdo->prepare('DELETE FROM registred WHERE userId=:userId AND courseId=:courseId');
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