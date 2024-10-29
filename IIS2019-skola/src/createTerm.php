<?php
    require_once 'functions/loader.php';

    
    if( isset($_SESSION['ISSET']) && ( isset($_POST['createTerm']) || isset($_POST['removeTerm']) ) )
    {
        if(isset($_POST['createTerm']))
        {
            $stmt = $pdo->prepare('INSERT INTO terms (name, type, score, datetime, description, courseId) VALUES(:name, :type, :score, :datetime, :description, :courseId)');
            $stmt->bindParam(':name', $_POST['name']);
            $stmt->bindParam(':type', $_POST['type']);
            $stmt->bindParam(':score', $_POST['score']);
            $stmt->bindParam(':datetime', $_POST['datetime']);
            $stmt->bindParam(':description', $_POST['description']);
            $stmt->bindParam(':courseId', $_POST['courseId']);
            $stmt->execute();
        }
        elseif(isset($_POST['removeTerm']))
        {
            /* $stmt = $pdo->prepare('DELETE FROM registred WHERE userId=:userId AND courseId=:courseId');
            $stmt->bindParam(':userId', $_POST['userId']);
            $stmt->bindParam(':courseId', $_POST['courseId']);
            $stmt->execute(); */
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