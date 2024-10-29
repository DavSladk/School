<?php
    require_once 'functions/loader.php';

    if(!isGuarantor() || !isset($_POST['courseId']))
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
        $tmp = $pdo->prepare('UPDATE courses SET name = :name, type = :type, tags = :tags, price = :price, description = :description WHERE courseId = :courseId');
        $tmp->bindParam(':name', $_POST['name']);
        $tmp->bindParam(':type', $_POST['type']);
        $tmp->bindParam(':tags', $_POST['tags']);
        $tmp->bindParam(':price', $_POST['price']);
        $tmp->bindParam(':description', $_POST['description']);
        $tmp->bindParam(':courseId', $_POST['courseId']);
        
        if($tmp->execute())
        {
            header('Location: courseDetail.php?id='.$_POST['courseId']);
        }
        else
        {
?>
            <h2>Wrong edit!</h2>
<?php
        }
    }

    //get course
    $courseDetail = $pdo->prepare('SELECT * FROM courses WHERE courseId=:id'); //approved
    $courseDetail->bindParam(':id', $_POST['courseId']);
    $courseDetail->execute();
    $courseDetailData = $courseDetail->fetch();

    //if term with id do not exists
    if(! $courseDetailData)
    {
?>
        <h1>No course with ID <?php echo $_POST['termId'] ?>!</h1>
<?php
        exit();
    }
?>

    <form method="post">
        <label>Name: </label><input type="text" name="name" value="<?php echo $courseDetailData['name'] ?>" required><br>
        <label>Type: </label><input type="text" name="type" value="<?php echo $courseDetailData['type'] ?>" required><br>
        <label>Tags: </label><input type="text" name="tags" value="<?php echo $courseDetailData['tags'] ?>" required><br>
        <label>Price: </label><input type="text" name="price" value="<?php echo $courseDetailData['price'] ?>" required><br>
        <label>Description: </label><input type="text" name="description" value="<?php echo $courseDetailData['description'] ?>" required><br>
        <input type="hidden" name="courseId" value="<?php echo $_POST['courseId'] ?>">
        <input type="submit" name="confirm" value="Confirm">
    </form>