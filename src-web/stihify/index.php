<?php

include_once("../common/config.php");

$page_title = "Stihify Results";

include_once(BASE_DIR."/common/head.php");

?>

<h1>Stihify Results</h1>


<h2 class="colsep">v0.3</h2>

<ul>
   <li>TPC layers:<br>

<?php
$layers = range(1, 45);

foreach($layers as $layer) {

	$layer_nice = sprintf("%02d", $layer);
   print "<a href=\"plots.php?pfx=v0.3&det=tpc_$layer_nice\">layer $layer_nice</a>";
	if ( !($layer % 13) )
		print "<br>\n";
	else
		print " &mdash; ";
}
?>

   <li><a href="plots.php?pfx=v0.3&det=sst_01">SST</a>
   <li><a href="plots.php?pfx=v0.3&det=ist_01">IST</a>
   <li><a href="plots.php?pfx=v0.3&det=pxl_02">PXL2</a>
   <li><a href="plots.php?pfx=v0.3&det=pxl_01">PXL1</a>

</ul>



<?php
include_once(BASE_DIR."/common/bottom.php");
?>
