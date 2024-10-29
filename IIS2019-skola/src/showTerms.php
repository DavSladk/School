<?php
    require_once 'functions/loader.php';

    if(!isset($_POST['showTerms']))
    {
?>
        <h1>Unauthorized access!</h1>
<?php
        exit();
    }
    require_once 'modules/header.php';

    $stmt = $pdo->prepare('SELECT T.termId, T.name, T.type, T.score, T.datetime  FROM terms T JOIN courses C ON T.courseId = C.courseId WHERE C.courseId = :courseId');
    $stmt->bindParam(':courseId', $_POST['courseId']);
    $stmt->execute();
?>
<h2>Terms</h2>
    <table>
    <tr>
    <th>Name</th>
    <th>Type</th>
    <th>Score</th>
    <th>Datetime</th>
    </tr>
<?php
    foreach($stmt as $row)
    {
?>
        <tr>
        <th><a href=<?php echo 'termDetail.php?id='.$row['termId']?>><?php echo $row['name']?></th>
        <th><?php echo $row['type']?></th>
        <th><?php echo $row['score']?></th>
        <th><?php echo $row['datetime']?></th>
        </tr>
<?php    
    }
?>
    </table>
