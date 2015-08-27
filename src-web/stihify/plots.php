<?php

include_once("../common/config.php");

$page_title = "Stihify Plots";

include_once(BASE_DIR."/common/head.php");
include_once(BASE_DIR."/common/PlotHelper.php");


$pfx = isset($_GET['pfx']) ? $_GET['pfx'] : "";

$info_file = STIHIFY_RESULTS_DIR."/".dirname($pfx)."/info.html";
$gP = new PlotHelper(STIHIFY_RESULTS_URL."/$pfx");

?>


<h1>Plots</h1>

<?php
if (file_exists($info_file))
   readfile($info_file);
?>


<div id="main_text_body">
<!-- {{{ -->



<h1 id="hit_eff" class="count">Single Hit Efficiency</h1>
<!-- {{{ -->
<div id="div:hit_eff" class="section">


<h2 id="hit_eff" class="count">Layer <?=$layer?></h2>
<!-- {{{ -->
<div id="div:hit_eff" class="section">

<p>
<table class="simple00 cntr">

<tr>
   <td><?php print $gP->img("deact_sst/sti_hit_any_node/hDiffProjToFitPositionWRTHit", 800); ?>

   <td><?php print $gP->img("deact_sst/sti_hit_any_node/hDiffProjToFitError", 800); ?>

<tr>
   <td><?php print $gP->img("deact_sst/sti_hit_any_node/hCountCandidateHits", 800); ?>

   <td><?php print $gP->img("deact_sst/sti_hit_any_node/hChi2CandidateHits", 800); ?>

<tr>
	<td>&nbsp;

   <td><?php print $gP->img("deact_sst/sti_hit_any_node/hPullCandidateHits2D", 800); ?>

<tr>
   <td><?php print $gP->img("deact_sst/sti_hit_any_node/hActiveLayerCounts", 800); ?>
   <div class="thumbcaption">
   Total number of track nodes. All sectors included
   </div>

   <td><?php print $gP->img("deact_sst/sti_hit_accepted/hActiveLayerCounts", 800); ?>
   <div class="thumbcaption">
   Number of track nodes with an accepted candidate hit. All sectors included
   </div>

<tr>
   <td><?php print $gP->img("deact_sst/sti_hit_ratio/hActiveLayerCounts_pxy", 800); ?>
   <div class="thumbcaption">
   Single hit efficiency in local detector coordinates (y and z). All sectors included
   </div>

   <td><?php print $gP->img("deact_sst/sti_hit_ratio/hActiveLayerCounts_pxy_pfx", 800); ?>
   <div class="thumbcaption">
   Single hit efficiency as a function of z (integrated over y). All sectors included
   </div>


</table>

</div>
<!-- }}} -->

</div>
<!-- }}} -->



</div> <!--main_text_body-->
<!-- }}} -->


<?php
include_once(BASE_DIR."/common/bottom.php");
?>
