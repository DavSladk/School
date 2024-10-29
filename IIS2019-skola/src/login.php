<?php
    require_once 'functions/loader.php';

    require_once 'modules/header.php';

    //if login form was filled
    if( isset($_POST['try']) )
    {
        //load data
        $stmt = $pdo->prepare('SELECT * FROM users WHERE login=:login AND pass=:pass');
        $stmt->bindParam(':login', $_POST['login']);
        $stmt->bindParam(':pass', $_POST['pass']);
        $stmt->execute();
        $data = $stmt->fetch();

        //check if user exists
        if($data)
        {
            //set up session
            $_SESSION['userId'] = $data['userId'];
            $_SESSION['login'] = $data['login'];
            $_SESSION['pass'] = $data['pass'];
            $_SESSION['name'] = $data['name'];
            $_SESSION['role'] = $data['role'];
            $_SESSION['ISSET'] = true;
            header('Location: home.php');
        }
        else
        {
            $fail = true;
        }
    }
?>
<h2>Login</h2>
<?php
    //if login failed
    if(isset($fail) && $fail === true)
    {
?>
        Wrong login/password
<?php
    }
?>
<!-- form for login -->
<form method="post">
    <label>Login: </label><input type="text" name="login" required><br>
    <label>Password: </label><input type="password" name="pass" required><br>
    <input type="submit" name="try" value="login">
</form>
<?php
    require_once 'modules/footer.php';
?>