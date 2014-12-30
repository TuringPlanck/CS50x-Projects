<?php

    // configuration
    require("../includes/config.php");
    
    // query users shares
    $rows = query("SELECT symbol, shares, price FROM `User Portfolios` WHERE id = ?", $_SESSION["id"]);
    
    // build table for user's shares
    $shares = [];
    foreach ($rows as $row)
    {
        $stock = lookup($row["symbol"]);
        if ($stock !== false)
        {
            $shares[] = [
                "name" => $stock["name"],
                "shares" => $row["shares"],
                "symbol" => $row["symbol"],
                "purchase_price" => $row["price"],
                "price_cur"    => $stock["price"],
                "total_bought" => $row["shares"] * $row["price"],
                "total_cur"    => $row["shares"] * $stock["price"],
                "profit"   => $row["shares"] * $stock["price"] - $row["shares"] * $row["price"]  
            ];
        }
    } 

    // render portfolio
    render("portfolio.php", ["shares" => $shares, "username" => $_SESSION["username"], "cash" => $_SESSION["cash"], "title" => "Portfolio"]);

?>
