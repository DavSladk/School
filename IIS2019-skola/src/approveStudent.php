<?php
    require_once 'functions/loader.php';

    
    if( isset($_SESSION['ISSET']) && ( isset($_POST['approve']) || isset($_POST['unapprove']) ) )
    {
        if(isset($_POST['approve']))
        {
            $stmt = $pdo->prepare('UPDATE registred SET approved = 1 WHERE userId = :userId AND courseId = :courseId');
            $stmt->bindParam(':userId', $_POST['userId']);
            $stmt->bindParam(':courseId', $_POST['courseId']);
            $stmt->execute();
        }
        elseif(isset($_POST['unapprove']))
        {
            $stmt = $pdo->prepare('UPDATE registred SET approved = 0 WHERE userId = :userId AND courseId = :courseId');
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