<?php
    require_once 'functions/loader.php';

    
    if( isset($_SESSION['ISSET']) && ( isset($_POST['createCourse']) || isset($_POST['removeCourse']) ) )
    {
        if(isset($_POST['createCourse']))
        {
            $stmt = $pdo->prepare('INSERT INTO courses (name, type, tags, price, description, guarantor) VALUES(:name, :type, :tags, :price, :description, :guarantor)');
            $stmt->bindParam(':name', $_POST['name']);
            $stmt->bindParam(':type', $_POST['type']);
            $stmt->bindParam(':tags', $_POST['tags']);
            $stmt->bindParam(':price', $_POST['price']);
            $stmt->bindParam(':description', $_POST['description']);
            $stmt->bindParam(':guarantor', $_POST['userId']);
            $stmt->execute();
        }
        elseif(isset($_POST['removeCourse']))
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

    header('Location: home.php');
?>