<?php
    // configuration
    require("../includes/config.php");
    
    // Query current users shares
    $rows = query("SELECT CASE WHEN type = 0 THEN 'SELL' ELSE 'BUY' END action, symbol, shares, price, date FROM history
        WHERE id = ? order by date desc", $_SESSION["id"]);
    
    if (count($rows) == 0)
    {
        apologize("You have no recorded transactions");
    }
    // render history
    render("history_display.php", ["title" => "History", "history" => $rows]);
?>
