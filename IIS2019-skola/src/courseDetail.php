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
    
    //get course
    $courseDetail = $pdo->prepare('SELECT C.name courseName, U.name userName, description, type, tags, price FROM courses C JOIN users U ON C.guarantor = U.userId WHERE courseId=:id'); //approved
    $courseDetail->bindParam(':id', $_GET['id']);
    $courseDetail->execute();
    $courseDetailData = $courseDetail->fetch();

    //if course with id do not exists
    if(! $courseDetailData)
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
            //is this user registred for this course?
            $tmp = $pdo->prepare('SELECT R.approved FROM registred R JOIN courses C ON R.courseId = C.courseId WHERE R.userId = :userId AND C.courseId = :courseId');
            $tmp->bindParam(':userId', $_SESSION['userId']);
            $tmp->bindParam(':courseId', $_GET['id']);
            $tmp->execute();
            $isRegistred = $tmp->fetch();
        }

        if(isLector())
        {
            //is this user lector for this course?
            $tmp = $pdo->prepare('SELECT * FROM lectors L JOIN courses C ON L.courseId = C.courseId WHERE L.userId = :userId AND C.courseId = :courseId');
            $tmp->bindParam(':userId', $_SESSION['userId']);
            $tmp->bindParam(':courseId', $_GET['id']);
            $tmp->execute();
            $isLector = $tmp->fetch();
        }
        
        if(isGuarantor())
        {
            //is this user guarantor for this course?
            $tmp = $pdo->prepare('SELECT * FROM courses C WHERE C.guarantor = :userId AND C.courseId = :courseId');
            $tmp->bindParam(':userId', $_SESSION['userId']);
            $tmp->bindParam(':courseId', $_GET['id']);
            $tmp->execute();
            $isGuarantor = $tmp->fetch();
        }
    }
        
    //get terms for this course
    $terms = $pdo->prepare('SELECT * FROM terms WHERE courseId = :courseId');
    $terms->bindParam(':courseId', $_GET['id']);
    $terms->execute();
    
    //get lectors for this course
    $lectors = $pdo->prepare('SELECT U.userId, U.name, U.login FROM lectors L JOIN users U ON L.userId = U.userId JOIN courses S ON L.courseId = S.courseId WHERE L.courseId = :courseId');
    $lectors->bindParam(':courseId', $_GET['id']);
    $lectors->execute();
    
    //get non lector for this course
    $nonLectors = false;
    if($isGuarantor)
    {
        $nonLectors = $pdo->prepare('SELECT U.userId, U.name FROM users U WHERE U.role IN ("lector", "guarantor", "supervisor", "administrator") AND U.userId NOT IN (SELECT L.userId FROM lectors L WHERE L.courseId = :courseId )');
        $nonLectors->bindParam(':courseId', $_GET['id']);
        $nonLectors->execute();
    }

    //get approved students
    $approvedStudents = false;
    if($isLector || $isGuarantor)
    {
        $approvedStudents = $pdo->prepare('SELECT * FROM users U JOIN registred R ON U.userId = R.userId WHERE R.courseId = :courseId AND R.approved = 1');
        $approvedStudents->bindParam(':courseId', $_GET['id']);
        $approvedStudents->execute();
    }

    //get unapproved students
    $unapprovedStudents = false;
    if($isGuarantor)
    {
        $unapprovedStudents = $pdo->prepare('SELECT * FROM users U JOIN registred R ON U.userId = R.userId WHERE R.courseId = :courseId AND R.approved = 0');
        $unapprovedStudents->bindParam(':courseId', $_GET['id']);
        $unapprovedStudents->execute();
    }
        
    require_once 'modules/header.php';

    //if student registration is not approved
    if($isRegistred)
    {
        if( $isRegistred['approved'] === '0' )
        {
?>
            <h2>Your registration is yet to be approved!</h2>
<?php
        }
    }    
?>
    <h1>Course details</h1>
    <!-- Detail block -->
    <h3>Name</h3>
        <?php echo $courseDetailData['courseName']?>
    <h3>Guarant</h3>
        <?php echo $courseDetailData['userName']?>
    <h3>Type</h3>
        <?php echo $courseDetailData['type']?>
    <h3>Tags</h3>
        <?php echo $courseDetailData['tags']?>
    <h3>Price</h3>
        <?php echo $courseDetailData['price']?> Bitcoin
    <h3>Description</h3>
        <?php echo $courseDetailData['description']?>

<!-- if guarantor, offer edit -->
<?php
    if($isGuarantor)
    {
?>
        <form action="editCourse.php" method="post">
            <input type="hidden" name="courseId" value="<?php echo $_GET['id'] ?>">
            <input type="submit" name="editCourse" value="Edit">
        </form>
<?php
    }
?>

<?php
    //if logged user, offer registration or unregistration
    if(isset($_SESSION['ISSET']) && isStudent())
    {
        if($isRegistred !== false)
        {
?>
            <!-- Unregister form -->
            <form action="registerCourse.php" method="post">
                <input type="hidden" name="userId" value="<?php echo $_SESSION['userId']?>">
                <input type="hidden" name="courseId" value="<?php echo $_GET['id']?>">
                <input type="submit" name="unregister" value="Unregister" />
            </form> 
            <?php
        }
        else
        {
            ?>
            <!-- Register form -->
            <form action="registerCourse.php" method="post">
                <input type="hidden" name="userId" value="<?php echo $_SESSION['userId']?>">
                <input type="hidden" name="courseId" value="<?php echo $_GET['id']?>">
                <input type="submit" name="register" value="Register" />
            </form>             
<?php
        }
    }
?>

    <!-- Term list block -->
    <h2>Term list</h2>
    <table>
        <tr>
            <th>Name</th>
            <th>Type</th>
            <th>Max Score</th>
            <th>Datetime</th>
        </tr>
<?php
        foreach($terms as $row)
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

<!-- If guarantor, offer creating few term -->
<?php
    if($isGuarantor)
    {
?>
        <h3>Create term</h3>
        <table>
            <tr>
                <th>Name</th>
                <th>Type</th>
                <th>Score</th>
                <th>Datetime</th>
                <th>Description</th>
            </tr>
            <tr>
                <form action="createTerm.php" method="post">
                    <th>
                        <input type="text" name="name" required/>
                    </th>
                    <th>
                        <input type="text" name="type" required/>
                    </th>
                    <th>
                        <input type="number" name="score" required/>
                    </th>
                    <th>
                        <input type="text" name="datetime" value="YYYY-MM-DD HH:MM:SS" required/>
                    </th>
                    <th>
                        <input type="text" name="description" required/>
                    </th>
                    <th>
                        <input type="hidden" name="courseId" value="<?php echo $_GET['id'] ?>">
                        <input type="submit" name="createTerm" value="Create term">
                    </th>
                </form>
            </tr>
        </table>
<?php
    }
?>

    <!-- Lector list -->
    <h2>Lectors</h2>
    <table>
        <tr>
            <th>Login</th>
            <th>Name</th>
            <!-- If guarantor, offer removal option -->
            <?php if($isGuarantor){ ?>
            <th>Remove</th>
            <?php } ?>
        </tr>
<?php
        foreach($lectors as $row)
        {
?>
            <tr>
                <th><?php echo $row['login']?></th>
                <th><?php echo $row['name']?></th>
<?php
            if($isGuarantor)
            {
?>
                <th>
                    <form method="post" action="assignLector.php">
                        <input type="hidden" name="userId" value="<?php echo $row['userId'] ?>">
                        <input type="hidden" name="courseId" value="<?php echo $_GET['id'] ?>">
                        <input type="submit" name="removeLector" value="Remove">
                    </form>
                </th>
<?php
            }
?>
            </tr>
<?php    
        }
?>
    </table>

<!-- Add lector -->
<?php
    if($isGuarantor)
    {
?>
        <form action="assignLector.php" method="post">
            <input type="hidden" name="courseId" value="<?php echo $_GET['id'] ?>">
            <select name="userId">
<?php
                foreach($nonLectors as $row)
                {
?>
                    <option value="<?php echo $row['userId'] ?>"><?php echo $row['name'] ?></option>
<?php
                }
?>
            </select>
            <input type="submit" name="addLector" value="Add lector">
        </form>
<?php
    }
?>

<!-- Approved Student list -->
<?php
    if($isLector || $isGuarantor)
    {
?>
    <h2>Approved students list</h2>
    <table>
        <tr>
            <th>Login</th>
            <th>Name</th>
            <th>Score</th>
            <?php if($isGuarantor){ ?> <th>Unapprove</th> <?php } ?>
        </tr>
<?php
        foreach($approvedStudents as $row)
        {
?>
            <tr>
            <th><?php echo $row['login']?></th>
            <th><?php echo $row['name']?></th>
            <th>
                <form method='post' action="changeScore.php">
                    <input type="number" min="0" max="100" name="score" value="<?php echo $row['score']?>"/>
                    <input type="hidden" name="registredId" value="<?php echo $row['registredId']?>" />
                    <input type="hidden" name="id" value="<?php echo $_GET['id']?>" />
                    <input type="submit" name="change" value="Change">
                </form>
            </th>
<?php
            if($isGuarantor)
            {
?>
                <th>
                    <form action="approveStudent.php" method="post">
                        <input type="hidden" name="userId" value="<?php echo $row['userId'] ?>">
                        <input type="hidden" name="courseId" value="<?php echo $_GET['id'] ?>">
                        <input type="submit" name="unapprove" value="Unapprove">
                    </form>
                </th>
<?php
            }
?>
            </tr>
<?php    
        }
    }
?>
    </table>

<?php
    if($isGuarantor)
    {
?>
    <!-- Unapproved Student list -->
    <h2>Unapproved students list</h2>
    <table>
        <tr>
            <th>Login</th>
            <th>Name</th>
            <th>Score</th>
            <th>Approve</th>
            <th>Unregister</th>
        </tr>
<?php
        foreach($unapprovedStudents as $row)
        {
?>
            <tr>
                <th><?php echo $row['login']?></th>
                <th><?php echo $row['name']?></th>
                <th><?php echo $row['score']?></th>
                <th>
                    <form method="post" action="approveStudent.php">
                        <input type="hidden" name="userId" value="<?php echo $row['userId'] ?>">
                        <input type="hidden" name="courseId" value="<?php echo $_GET['id'] ?>">
                        <input type="submit" name="approve" value="Approve">
                    </form>
                </th>
                <th>
                    <form method="post" action="registerCourse.php">
                        <input type="hidden" name="userId" value="<?php echo $row['userId'] ?>">
                        <input type="hidden" name="courseId" value="<?php echo $_GET['id'] ?>">
                        <input type="submit" name="unregister" value="Unregister">
                    </form>
                </th>
            </tr>
<?php    
        }
    }
?>
    </table>

<?php
    require_once 'modules/footer.php';
?>