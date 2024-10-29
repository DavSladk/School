<?php
    require_once 'functions/loader.php';
    if(! isLector() || ! isset($_POST['score']) || ! isset($_POST['registredId']) || ! isset($_POST['change']) )
    {
?>
        <h1>Unauthorized access!</h1>
<?php
        exit();
    }
    $tmp = $pdo->prepare('UPDATE registred SET score = :score WHERE registredId = :registredId');
    $tmp->bindParam(':score', $_POST['score']);
    $tmp->bindParam(':registredId', $_POST['registredId']);
    $tmp->execute();
    header('Location: courseDetail.php?id='.$_POST['id']);
?>