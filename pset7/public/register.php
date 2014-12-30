<?php

    // configuration
    require("../includes/config.php");

    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // if username and password fields are empty
        if (($_POST["username"] == "") || ($_POST["password"] == "") || ($_POST["confirmation"] == ""))
        {
            apologize("Please complete all fields before submission"); 
        }
        // if passwords do not match
        else if ($_POST["password"] != $_POST["confirmation"])
        {
            apologize("Passwords do not match. Please reconfirm submission.");
        }
        else
        {
            // attempt to add new user
            $result = query("INSERT INTO users (username, hash, cash) VALUES(?, ?, 10000.00)", $_POST["username"], crypt($_POST["password"]));
            
            // apologize if query fails
            if ($result === false)
            {
                apologize("Invalid entry. Possible duplicate username. Please try different registration paramaters.");
            }
            // else, log-in user
            else
            {
                 // query database for user
                $rows = query("SELECT * FROM users WHERE username = ?", $_POST["username"]);

                // if we found user, check password
                if (count($rows) == 1)
                {
                    // first (and only) row
                    $row = $rows[0];

                    // compare hash of user's input against hash that's in database
                    if (crypt($_POST["password"], $row["hash"]) == $row["hash"])
                    {
                        // remember that user's now logged in by storing user's ID, username, cash in session
                        $_SESSION["id"] = $row["id"];
                        $_SESSION["username"] = $row["username"];
                        $_SESSION["cash"] = $row["cash"];

                        // redirect to portfolio
                        redirect("/");
                    }
                }
            }
        }
    }
    else
    {
        // else render form
        render("register_form.php", ["title" => "Register"]);
    }

?>
