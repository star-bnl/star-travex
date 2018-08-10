<?php

include_once("../common/config.php");

$page_title = "Stihify Results";

include_once(BASE_DIR."/common/head.php");

?>

<h1>Stihify Results</h1>


<h2 class="colsep">v0.4b</h2>

<ul>
   <li><a href="plots.php?pfx=v0.4b&det=sst">SST</a> - <a href="plots.php?pfx=v0.4b&det=sst_hft_hits">SST (tracks with HFT hits)</a>
   <li><a href="plots.php?pfx=v0.4b&det=ist">IST</a> - <a href="plots.php?pfx=v0.4b&det=ist_hft_hits">IST (tracks with HFT hits)</a>
   <li><a href="plots.php?pfx=v0.4b&det=pxl2">PXL2</a> - <a href="plots.php?pfx=v0.4b&det=pxl2_hft_hits">PXL2 (tracks with HFT hits)</a>
   <li><a href="plots.php?pfx=v0.4b&det=pxl1">PXL1</a> - <a href="plots.php?pfx=v0.4b&det=pxl1_hft_hits">PXL1 (tracks with HFT hits)</a>
</ul>


<h2 class="colsep">v0.4a</h2>

<ul>
   <li><a href="plots.php?pfx=v0.4a&det=sst">SST</a>
</ul>


<h2 class="colsep">v0.3b</h2>

<ul>
   <li>TPC layers:<br>

<?php
$layers = range(1, 45);

foreach($layers as $layer) {

   $layer_nice = sprintf("%02d", $layer);
   print "<a href=\"plots.php?pfx=v0.3b&det=tpc_$layer_nice\">layer $layer_nice</a>";
   if ( !($layer % 13) )
      print "<br>\n";
   else
      print " &mdash; ";
}
?>

</ul>


<h2 class="colsep">v0.3a</h2>

<ul>
   <li>TPC layers:<br>

<?php
$layers = range(1, 45);

foreach($layers as $layer) {

   $layer_nice = sprintf("%02d", $layer);
   print "<a href=\"plots.php?pfx=v0.3a&det=tpc_$layer_nice\">layer $layer_nice</a>";
   if ( !($layer % 13) )
      print "<br>\n";
   else
      print " &mdash; ";
}
?>

   <li><a href="plots.php?pfx=v0.3a&det=sst_01">SST</a>
   <li><a href="plots.php?pfx=v0.3a&det=ist_01">IST</a>
   <li><a href="plots.php?pfx=v0.3a&det=pxl_02">PXL2</a>
   <li><a href="plots.php?pfx=v0.3a&det=pxl_01">PXL1</a>

</ul>



<?php
include_once(BASE_DIR."/common/bottom.php");
?>