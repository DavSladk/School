<?php
    require_once 'functions/loader.php';

    if(!isLector() || !isset($_POST['termId']))
    {
?>
        <h1>Unauthorized access!</h1>
<?php
        exit();
    }

    $success = false;
    //if editting, insert
    if(isset($_POST['confirm']))
    {
        $tmp = $pdo->prepare('UPDATE terms SET name = :name, type = :type, score = :score, datetime = :datetime, description = :description WHERE termId = :termId');
        $tmp->bindParam(':name', $_POST['name']);
        $tmp->bindParam(':type', $_POST['type']);
        $tmp->bindParam(':score', $_POST['score']);
        $tmp->bindParam(':datetime', $_POST['datetime']);
        $tmp->bindParam(':description', $_POST['description']);
        $tmp->bindParam(':termId', $_POST['termId']);
        
        if($tmp->execute())
        {
            header('Location: termDetail.php?id='.$_POST['termId']);
        }
        else
        {
?>
            <h2>Wrong edit!</h2>
<?php
        }
    }

    //get term
    $termDetail = $pdo->prepare('SELECT * FROM terms WHERE termId=:id'); //approved
    $termDetail->bindParam(':id', $_POST['termId']);
    $termDetail->execute();
    $termDetailData = $termDetail->fetch();

    //if term with id do not exists
    if(! $termDetailData)
    {
?>
        <h1>No course with ID <?php echo $_POST['termId'] ?>!</h1>
<?php
        exit();
    }
?>

    <form method="post">
        <label>Name: </label><input type="text" name="name" value="<?php echo $termDetailData['name'] ?>" required><br>
        <label>Type: </label><input type="text" name="type" value="<?php echo $termDetailData['type'] ?>" required><br>
        <label>Max score: </label><input type="text" name="score" value="<?php echo $termDetailData['score'] ?>" required><br>
        <label>Datatime: </label><input type="text" name="datetime" value="<?php echo $termDetailData['datetime'] ?>" required><br>
        <label>Description: </label><input type="text" name="description" value="<?php echo $termDetailData['description'] ?>" required><br>
        <input type="hidden" name="termId" value="<?php echo $_POST['termId'] ?>">
        <input type="submit" name="confirm" value="Confirm">
    </form>