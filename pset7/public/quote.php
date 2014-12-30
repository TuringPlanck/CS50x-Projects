<?php

    // configuration
    require("../includes/config.php");

    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // check that symbol was submitted
        if ($_POST["symbol"] == "")
        {
            apologize("Please enter a stock symbol to quote");
        }
        
        // lookup stock symbol
        $stock = lookup($_POST["symbol"]);
        
        // validate stock symbol
        if ($_POST["symbol"] === false)
        {
            apologize("Stock symbol does not exist. Please try again.");
        }
        else
        {
            // render stock quote
            render("quote_display.php", ["title" => "Quote", "symbol" => $stock["symbol"], "name" => $stock["name"], "price" => $stock["price"]]);
        }   
    }
    else
    {
        // else render form
        render("quote_form.php", ["title" => "Quote"]);
    }

?>
