<div>
    <ul class="nav nav-pills">
        <li><a href="quote.php">Quote</a></li>
        <li><a href="buy.php">Buy</a></li>
        <li><a href="sell.php">Sell</a></li>
        <li><a href="history.php">History</a></li>
        <li><a href="logout.php"><strong>Log Out</strong></a></li>
    </ul>
</div>
<div>
    <table class="table table-hover">
        <thead>
        <!--
            <tr>
                <th rowspan = 2>Symbol</th>
                <th rowspan = 2>Name</th>
                <th rowspan = 2>Shares</th>
                <th rowspan = 1 colspan = 2>Price</th>
                <th rowspan = 1 colspan = 2>Total</th>
                <th rowspan = 2>Total gains / losses</th>
            </tr>
            <tr>
                <th rowspan = 1>Bought</th>
                <th rowspan = 1>Current</th>
                <th rowspan = 1>Bought</th>
                <th rowspan = 1>Current</th>
            </tr>
        -->
            <tr>
                <th>Symbol</th>
                <th>Name</th>
                <th>Shares</th>
                <th>Purchase Price</th>
                <th>Current Price</th>
                <th>Purchase Value</th>
                <th>Current Value</th>
                <th>Total gains / losses</th>
            </tr>
        </thead>
        <tbody>
            <?php foreach($shares as $share): ?>
                <tr>
                    <td><?= $share["symbol"] ?></td>
                    <td><?= $share["name"] ?></td>
                    <td><?= $share["shares"] ?></td>
                    <td><?= number_format($share["purchase_price"], 2) ?></td>
                    <td><?= number_format($share["price_cur"], 2) ?></td>
                    <td><?= number_format($share["total_bought"], 2) ?></td>
                    <td><?= number_format($share["total_cur"], 2) ?></td>
                    <td><?= number_format($share["profit"], 2) ?></td>
                </tr>
            <? endforeach ?>
        </tbody>
    </table>
</div>
<div>
    <a href="logout.php">Log Out</a>
</div>
<div>
    <a href="reset.php">Password Reset</a>
</div>
