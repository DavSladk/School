<?php
    require_once 'functions/loader.php';
    
    $queryResult = false;
    if( isset($_GET['filter']) && isset($_GET['search']) )
    {
        //get search results
        $queryResult = $pdo->prepare('SELECT * FROM courses WHERE name LIKE :filter OR type LIKE :filter OR tags LIKE :filter');
        $filter = $_GET['filter'];
        $filter = "%$filter%";
        $queryResult->bindParam(':filter', $filter);
        $queryResult->execute();
    }
    else
    {
        //get all courses
        $queryResult = $pdo->query('SELECT * FROM courses'); //approved
    }

    require_once 'modules/header.php';

//generate courses table 
?>
    <h2>All Courses</h2>
    <table>
    <tr>
    <th>Name</th>
    <th>Type</th>
    <th>Price</th>
    </tr>
    <form method="get">
        Search: <input type="search" name="filter"/><input type="submit" name="search" value="Search"><br>
    </form>
<?php
    foreach($queryResult as $row)
    {
?>
        <tr>
        <th><a href=<?php echo 'courseDetail.php?id='.$row['courseId']?>><?php echo $row['name']?></th>
        <th><?php echo $row['type']?></th>
        <th><?php echo $row['price']?> Bitcoin</th>
        </tr>
<?php    
    }
?>

<?php
    if(isGuarantor())
    {
        ?>
        <h3>Create course</h3>
        <table>
            <tr>
                <th>Name</th>
                <th>Type</th>
                <th>Tags</th>
                <th>Price</th>
                <th>Description</th>
            </tr>
            <tr>
                <form action="createCourse.php" method="post">
                    <th>
                        <input type="text" name="name" required/>
                    </th>
                    <th>
                        <input type="text" name="type" required/>
                    </th>
                    <th>
                        <input type="text" name="tags" required/>
                    </th>
                    <th>
                        <input type="number" name="price" required/>
                    </th>
                    <th>
                        <input type="text" name="description" required/>
                    </th>
                    <th>
                        <input type="hidden" name="userId" value="<?php echo $_SESSION['userId'] ?>">
                        <input type="submit" name="createCourse" value="Create course">
                    </th>
                </form>
            </tr>
        </table>
<?php       
    }
?>

<?php
    require_once 'modules/footer.php';
?>