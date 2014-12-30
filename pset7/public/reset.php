<?php
    
    // configuration
    require("../includes/config.php"); 
    
    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        
        // validate submission
        if (empty($_POST["new_pass"] || empty($_POST["confirmation"])))
        {
            apologize("Please complete all fields before submission");
        }
        else
        {
            // reset password
            $result = query("UPDATE users SET hash = ? where id = ?", crypt($_POST["new_pass"]), $_SESSION["id"]);
            
            // check for errors
            if ($result === false)
            {
                apologize("An error ocurred.");
            }
            // else, log-in user
            else
            {
                redirect("/");
            }
        }
    }
    else
    {
        // else, render form
        render("reset_form.php", ["title" => "Forgot Password"]);
    }
?>
