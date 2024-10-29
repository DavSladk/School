<?php
    require_once 'functions/loader.php';

    //checking if logged user
    if(!isset($_GET['id']))
    {
?>
        <h1>Unauthorized access!</h1>
<?php
        exit();
    }

    //get term
    $termDetail = $pdo->prepare('SELECT * FROM terms WHERE termId=:id'); //approved
    $termDetail->bindParam(':id', $_GET['id']);
    $termDetail->execute();
    $termDetailData = $termDetail->fetch();

    //if term with id do not exists
    if(! $termDetailData)
    {
?>
        <h1>No course with ID <?php echo $_GET['id'] ?>!</h1>
<?php
        exit();
    }

    $isRegistred = false;
    $isLector = false;
    $isGuarantor = false;
    if(isset($_SESSION['ISSET']))
    {
        if(isStudent())
        {
            //is this user registred for this term?
            $tmp = $pdo->prepare('SELECT * FROM terms T JOIN registred R ON T.courseId = R.courseId WHERE R.userId = :userId AND T.termId = :termId');
            $tmp->bindParam(':userId', $_SESSION['userId']);
            $tmp->bindParam(':termId', $_GET['id']);
            $tmp->execute();
            $isRegistred = $tmp->fetch();
        }

        if(isLector())
        {
            //is this user registred for this term?
            $tmp = $pdo->prepare('SELECT * FROM terms T JOIN lectors L ON T.courseId = L.courseId WHERE T.termId = :termId AND L.userId = :userId');
            $tmp->bindParam(':userId', $_SESSION['userId']);
            $tmp->bindParam(':termId', $_GET['id']);
            $tmp->execute();
            $isLector = $tmp->fetch();
        }
        
        if(isGuarantor())
        {
            //is this user guarantor for this course?
            $tmp = $pdo->prepare('SELECT * FROM terms T JOIN courses C ON T.courseId = C.courseId WHERE T.termId = :termId AND C.guarantor = :userId');
            $tmp->bindParam(':userId', $_SESSION['userId']);
            $tmp->bindParam(':termId', $_GET['id']);
            $tmp->execute();
            $isGuarantor = $tmp->fetch();
        }
    }
    
    //get files for this term
    $files = $pdo->prepare('SELECT * FROM files WHERE termId = :termId');
    $files->bindParam(':termId', $_GET['id']);
    $files->execute();

    require_once 'modules/header.php';
?>
    <!-- Detail block -->
    <h1>Term details</h1>
    <h3>Name</h3>
        <?php echo $termDetailData['name']?>
    <h3>Type</h3>
        <?php echo $termDetailData['type']?>
    <h3>Max Score</h3>
        <?php echo $termDetailData['score']?>
    <h3>Datetime</h3>
        <?php echo $termDetailData['datetime']?>
    <h3>Description</h3>
        <?php echo $termDetailData['description']?>
<?php
    //if lector or guarantor, offer mod option
    if($isLector || $isGuarantor)
    {
?>
        <form action="editTerm.php" method="post">
            <input type="hidden" name="termId" value="<?php echo $_GET['id'] ?>">
            <input type="submit" name="editTerm" value="Edit">
        </form>
<?php
    }
?>


<!-- File list -->
<?php
    if($isRegistred || $isLector || $isGuarantor)
    {
?>
        <h2>Files</h2>
        <table>
            <tr>
                <th>Name</th>
            </tr>
<?php
        foreach($files as $row)
        {
?>
            <tr>
                <th><a href=<?php echo '../files/'.$_GET['id']."/".$row['fileName']?> download><?php echo $row['fileName']?></th>
            </tr>
<?php    
        }
?>
        </table>
<?php
    }
?>

<!-- Add file -->
<?php
    if($isLector || $isGuarantor)
    {
        $_SESSION['id'] = $_GET['id'];
?>
        <form action="uploadFile.php" method="post" enctype="multipart/form-data">
            <input type="hidden" name="termId" value="<?php echo $_GET['id'] ?>">
            <input type="file" name="fileToUpload" required>
            <input type="submit" value="Upload File" name="submit">
        </form>
<?php
    }
?>

<?php
    require_once 'modules/footer.php';
?>