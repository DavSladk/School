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

    //get ordered terms
    $terms = $pdo->prepare('SELECT * FROM registred R JOIN terms T ON R.courseId = T.courseId WHERE R.userId = :userId ORDER BY T.datetime ASC');
    $terms->bindParam(':userId', $_SESSION['userId']);
    $terms->execute();

    require_once 'modules/header.php';
?>
    <!-- Term list block -->
    <h2>Shedule</h2>
    <table>
        <tr>
            <th>Datetime</th>
            <th>Name</th>
            <th>Type</th>
            <th>Max Score</th>
        </tr>
<?php
        foreach($terms as $row)
        {
?>
            <tr>
                <th><?php echo $row['datetime']?></th>
                <th><a href=<?php echo 'termDetail.php?id='.$row['termId']?>><?php echo $row['name']?></th>
                <th><?php echo $row['type']?></th>
                <th><?php echo $row['score']?></th>
            </tr>
<?php    
        }
?>
    </table>
<?php
    require_once 'modules/footer.php';
?>