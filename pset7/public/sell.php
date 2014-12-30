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
        
        // Query Yahoo
        $stock = lookup($_POST["symbol"]);
        
        // Validate Shares
        if ($stock === false)
        {
            apologize("Please enter a valid quantity to sell");
        }
        else
        {
            // Query the user's shares
            $rows = query("SELECT shares FROM `User Portfolios` WHERE id = ? and symbol = ?", $_SESSION["id"], strtoupper($_POST["symbol"]));
            if (count($rows) == 1)
            {
                $shares = $rows[0]["shares"];
            }
            else
            {
                apologize("Shares for specified symbol not found.");
            }
        
            $value = $stock["price"] * $shares;
            
            // Remove stock from user's portfolio
            $query = query("DELETE FROM `User Portfolios` where id = ? and symbol = ?", $_SESSION["id"], strtoupper($_POST["symbol"]));
            
            if ($query === false)
            {
                apologize("An error ocurred");
            }
            
            // Update user's cash
            $query = query("UPDATE users SET cash = cash + ? where id = ?", $value, $_SESSION["id"]);
            if ($query === false)
            {
               apologize("An error ocurred");
            }
               
            $_SESSION["cash"] += $value;
            
            // Log the history
            $query = query("INSERT INTO history(id, type, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?, Now())"
                ,$_SESSION["id"], 0, strtoupper($_POST["symbol"]), $shares, $stock["price"]);
            
            // Redirect to home
            redirect("/");
        }
    }
    else
    {
        // Fill the array of user shares
        $rows = query("SELECT symbol FROM `User Portfolios` WHERE id = ?", $_SESSION["id"]);
        render("sell_form.php", ["title" => "Sell", "symbols" => $rows]);
    }
?>
