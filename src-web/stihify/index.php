<?php

include_once("../common/config.php");

$page_title = "Stihify Results";

include_once(BASE_DIR."/common/head.php");

?>

<h1>Stihify Results</h1>

<h2 class="colsep">v0.2</h2>

<ul>
   <li><a href="plots_tpc.php?pfx=v0.2">TPC</a>
   <li><a href="plots.php?pfx=v0.2&det=sst">SST</a>
   <li><a href="plots.php?pfx=v0.2&det=ist">IST</a>
   <li><a href="plots.php?pfx=v0.2&det=pxl2">PXL2</a>
   <li><a href="plots.php?pfx=v0.2&det=pxl1">PXL1</a>
</ul>



<?php
include_once(BASE_DIR."/common/bottom.php");
?>
