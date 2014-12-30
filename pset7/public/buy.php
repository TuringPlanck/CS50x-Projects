<?php
    
    // configuration
    require("../includes/config.php"); 
    
    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // Validate the name
        if ($_POST["symbol"] == "")
        {
            apologize("Please enter a valid stock symbol.");
        }
        
        // Validate the shares
        if (($_POST["shares"] == "") || !is_numeric($_POST["shares"]) || !preg_match("/^\d+$/", $_POST["shares"]))
        {
            apologize("Please enter a valid quantity to buy");
        }
        
        // Query Yahoo
        $stock = lookup($_POST["symbol"]);
        
        // Validate stock symbol
        if ($stock === false)
        {
            apologize("Please enter a valid stock symbol.");
        }
        else
        {
            $value = $stock["price"] * $_POST["shares"];
            
            // Ensure user has sufficient cash for purchase
            if ($_SESSION["cash"] < $value)
            {
                apologize("You do not have the funds to purchase these shares");
            }
            else
            {
                // Add purchased shares to database
                $query = query("INSERT INTO `User Portfolios`(id, symbol, shares, price) VALUES (?, ?, ?, ?)
                    ON DUPLICATE KEY UPDATE shares = shares + VALUES(shares), price = VALUES(price)"
                    ,$_SESSION["id"], strtoupper($stock["symbol"]), $_POST["shares"], $stock["price"]);
                if ($query === false)
                {
                    apologize("An error occurred");
                }
                
                // Update the user's cash
                $query = query("UPDATE users SET cash = cash - ? where id = ?", $value, $_SESSION["id"]);
                if ($query === false)
                {
                    apologize("An error occurred");
                }
              
                $_SESSION["cash"] -= $value;
                
                // Log the history
                $query = query("INSERT INTO history(id, type, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?, Now())"
                    ,$_SESSION["id"], 1, strtoupper($stock["symbol"]), $_POST["shares"], $stock["price"]);
                
                // Redirect to home
                redirect("index.php");
            }
        }
    }
    else
    {
        // else render form
        render("buy_form.php", ["title" => "Buy"]);
    }
?>
