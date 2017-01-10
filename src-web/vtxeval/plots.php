<?php

include_once("../common/config.php");

$page_title = "Vtxeval Plots";

include_once(BASE_DIR."/common/head.php");
include_once(BASE_DIR."/common/PlotHelper.php");


$pfx = isset($_GET['pfx']) ? $_GET['pfx'] : "";
$det = isset($_GET['det']) ? $_GET['det'] : "";

$info_file = VTXEVAL_RESULTS_DIR."/".dirname($pfx).".lis_result/info.html";
$gP = new PlotHelper(VTXEVAL_RESULTS_URL."/$pfx.lis_result", "");

?>


<?php
if (file_exists($info_file))
   readfile($info_file);
?>


<div id="main_text_body">
<!-- {{{ -->


<div class="count" id="generated-toc"></div>

<p>&nbsp;</p>


<h1 id="vertex_finder" class="count">Vertex Finder: <?=$pfx?></h1>
<!-- {{{ -->



<h2 id="h_event" class="count">Event Histograms</h2>
<!-- {{{ -->
<div id="div:h_event" class="section">


<p>
<table class="simple00 cntr">

<tr>
   <td><?php print $gP->img("event/hNumTracksPerEvent"); ?>
   <div class="thumbcaption">
   Total number of tracks in event
   </div>

   <td><?php print $gP->img("event/hNumTracksPerEventPrimary"); ?>
   <div class="thumbcaption">
   Total number of primary tracks in event
   </div>

   <td><?php print $gP->img("event/hNumVertices"); ?>
   <div class="thumbcaption">
   Total number of vertices
   </div>

<tr>
   <td>&nbsp;

   <td><?php print $gP->img("event/McRecMulAny_eff"); ?>
   <div class="thumbcaption">
   Primary vertex reconstruction efficiency
   </div>

   <td><?php print $gP->img("event/McRecMulGood_eff"); ?>
   <div class="thumbcaption">
   Reconstruction efficiency for the maximum rank vertex
   </div>

</table>


</div>
<!-- }}} -->



<h2 id="h_vertex_mc" class="count">Simulated Vertex (matching reco vertex)</h2>
<!-- {{{ -->
<div id="div:h_vertex_mc" class="section">


<p>
<table class="simple00 cntr">

<tr>
   <td><?php print $gP->img("vertex/hMcVertexX"); ?>
   <div class="thumbcaption">
   Vertex position X
   </div>

   <td>&nbsp;

   <td><?php print $gP->img("vertex/hMcVertexZvX"); ?>
   <div class="thumbcaption">
   Vertex position Z vs X
   </div>

<tr>

   <td><?php print $gP->img("vertex/hMcVertexXvY"); ?>
   <div class="thumbcaption">
   Vertex position X vs Y
   </div>

   <td><?php print $gP->img("vertex/hMcVertexY"); ?>
   <div class="thumbcaption">
   Vertex position Y
   </div>

   <td><?php print $gP->img("vertex/hMcVertexZvY"); ?>
   <div class="thumbcaption">
   Vertex position Z vs Y
   </div>

<tr>
   <td>&nbsp;

   <td>&nbsp;

   <td><?php print $gP->img("vertex/hMcVertexZ"); ?>
   <div class="thumbcaption">
   Vertex position Z
   </div>

</table>


</div>
<!-- }}} -->



<h2 id="h_vertex_reco" class="count">Reconstructed Vertex</h2>
<!-- {{{ -->
<div id="div:h_vertex_reco" class="section">


<p>
<table class="simple00 cntr">

<tr>
   <td><?php print $gP->img("vertex/hVertexX"); ?>
   <div class="thumbcaption">
   Vertex position X
   </div>

   <td>&nbsp;

   <td><?php print $gP->img("vertex/hVertexZvX"); ?>
   <div class="thumbcaption">
   Vertex position Z vs X
   </div>

<tr>

   <td><?php print $gP->img("vertex/hVertexXvY"); ?>
   <div class="thumbcaption">
   Vertex position X vs Y
   </div>

   <td><?php print $gP->img("vertex/hVertexY"); ?>
   <div class="thumbcaption">
   Vertex position Y
   </div>

   <td><?php print $gP->img("vertex/hVertexZvY"); ?>
   <div class="thumbcaption">
   Vertex position Z vs Y
   </div>

<tr>
   <td>&nbsp;

   <td>&nbsp;

   <td><?php print $gP->img("vertex/hVertexZ"); ?>
   <div class="thumbcaption">
   Vertex position Z
   </div>

<tr>
   <td><?php print $gP->img("vertex/hVertexErrorX"); ?>
   <div class="thumbcaption">
   Vertex position error X
   </div>

   <td><?php print $gP->img("vertex/hVertexErrorY"); ?>
   <div class="thumbcaption">
   Vertex position error Y
   </div>

   <td><?php print $gP->img("vertex/hVertexErrorZ"); ?>
   <div class="thumbcaption">
   Vertex position error Z
   </div>

</table>


</div>
<!-- }}} -->



<h2 id="h_vertex_reco_hft" class="count">Reconstructed Vertex (HFT tracks)</h2>
<!-- {{{ -->
<div id="div:h_vertex_reco_hft" class="section">


<p>
<table class="simple00 cntr">

<tr>
   <td><?php print $gP->img("vertex_hft/hVertexX"); ?>
   <div class="thumbcaption">
   Vertex position X
   </div>

   <td>&nbsp;

   <td><?php print $gP->img("vertex_hft/hVertexZvX"); ?>
   <div class="thumbcaption">
   Vertex position Z vs X
   </div>

<tr>

   <td><?php print $gP->img("vertex_hft/hVertexXvY"); ?>
   <div class="thumbcaption">
   Vertex position X vs Y
   </div>

   <td><?php print $gP->img("vertex_hft/hVertexY"); ?>
   <div class="thumbcaption">
   Vertex position Y
   </div>

   <td><?php print $gP->img("vertex_hft/hVertexZvY"); ?>
   <div class="thumbcaption">
   Vertex position Z vs Y
   </div>

<tr>
   <td>&nbsp;

   <td>&nbsp;

   <td><?php print $gP->img("vertex_hft/hVertexZ"); ?>
   <div class="thumbcaption">
   Vertex position Z
   </div>

<tr>
   <td><?php print $gP->img("vertex_hft/hVertexErrorX"); ?>
   <div class="thumbcaption">
   Vertex position error X
   </div>

   <td><?php print $gP->img("vertex_hft/hVertexErrorY"); ?>
   <div class="thumbcaption">
   Vertex position error Y
   </div>

   <td><?php print $gP->img("vertex_hft/hVertexErrorZ"); ?>
   <div class="thumbcaption">
   Vertex position error Z
   </div>

</table>


</div>
<!-- }}} -->



<h2 id="h_vertex_mc_reco" class="count">Reco vs Simu Vertex</h2>
<!-- {{{ -->
<div id="div:h_vertex_mc_reco" class="section">


<p>
<table class="simple00 cntr">

<tr>
   <td><?php print $gP->img("vertex/hVertexPullX"); ?>
   <div class="thumbcaption">
   Vertex pull distribuition in X
   </div>

   <td><?php print $gP->img("vertex/hVertexPullY"); ?>
   <div class="thumbcaption">
   Vertex pull distribuition in Y
   </div>

   <td><?php print $gP->img("vertex/hVertexPullZ"); ?>
   <div class="thumbcaption">
   Vertex pull distribuition in Z
   </div>

<tr>
   <td><?php print $gP->img("vertex/hMcRecoVertexDeltaX"); ?>
   <div class="thumbcaption">
   Delta X: Simu - Reco
   </div>

   <td><?php print $gP->img("vertex/hMcRecoVertexDeltaY"); ?>
   <div class="thumbcaption">
   Delta Y: Simu - Reco
   </div>

   <td><?php print $gP->img("vertex/hMcRecoVertexDeltaZ"); ?>
   <div class="thumbcaption">
   Delta Z: Simu - Reco
   </div>

<tr>
   <td>&nbsp;

   <td><?php print $gP->img("vertex/hMcRecoVertexDelta"); ?>
   <div class="thumbcaption">
   Distance: Simu - Reco
   </div>

   <td>&nbsp;

</table>


</div>
<!-- }}} -->



<h2 id="h_vertex_mc_reco_hft" class="count">Reco vs Simu Vertex (HFT tracks)</h2>
<!-- {{{ -->
<div id="div:h_vertex_mc_reco_hft" class="section">


<p>
<table class="simple00 cntr">

<tr>
   <td><?php print $gP->img("vertex_hft/hVertexPullX"); ?>
   <div class="thumbcaption">
   Vertex pull distribuition in X
   </div>

   <td><?php print $gP->img("vertex_hft/hVertexPullY"); ?>
   <div class="thumbcaption">
   Vertex pull distribuition in Y
   </div>

   <td><?php print $gP->img("vertex_hft/hVertexPullZ"); ?>
   <div class="thumbcaption">
   Vertex pull distribuition in Z
   </div>

<tr>
   <td><?php print $gP->img("vertex_hft/hMcRecoVertexDeltaX"); ?>
   <div class="thumbcaption">
   Delta X: Simu - Reco
   </div>

   <td><?php print $gP->img("vertex_hft/hMcRecoVertexDeltaY"); ?>
   <div class="thumbcaption">
   Delta Y: Simu - Reco
   </div>

   <td><?php print $gP->img("vertex_hft/hMcRecoVertexDeltaZ"); ?>
   <div class="thumbcaption">
   Delta Z: Simu - Reco
   </div>

<tr>
   <td>&nbsp;

   <td><?php print $gP->img("vertex_hft/hMcRecoVertexDelta"); ?>
   <div class="thumbcaption">
   Distance: Simu - Reco
   </div>

   <td>&nbsp;

</table>


</div>
<!-- }}} -->



<h2 id="h_vertex_max_rank" class="count">Max Rank Reconstructed Vertex</h2>
<!-- {{{ -->
<div id="div:h_vertex_max_rank" class="section">


<p>
<table class="simple00 cntr">

<tr>
   <td><?php print $gP->img("vertex_maxrank/hVertexX"); ?>
   <div class="thumbcaption">
   Vertex position X
   </div>

   <td>&nbsp;

   <td><?php print $gP->img("vertex_maxrank/hVertexZvX"); ?>
   <div class="thumbcaption">
   Vertex position Z vs X
   </div>

<tr>

   <td><?php print $gP->img("vertex_maxrank/hVertexXvY"); ?>
   <div class="thumbcaption">
   Vertex position X vs Y
   </div>

   <td><?php print $gP->img("vertex_maxrank/hVertexY"); ?>
   <div class="thumbcaption">
   Vertex position Y
   </div>

   <td><?php print $gP->img("vertex_maxrank/hVertexZvY"); ?>
   <div class="thumbcaption">
   Vertex position Z vs Y
   </div>

<tr>
   <td>&nbsp;

   <td>&nbsp;

   <td><?php print $gP->img("vertex_maxrank/hVertexZ"); ?>
   <div class="thumbcaption">
   Vertex position Z
   </div>

<tr>
   <td><?php print $gP->img("vertex_maxrank/hVertexErrorX"); ?>
   <div class="thumbcaption">
   Vertex position error X
   </div>

   <td><?php print $gP->img("vertex_maxrank/hVertexErrorY"); ?>
   <div class="thumbcaption">
   Vertex position error Y
   </div>

   <td><?php print $gP->img("vertex_maxrank/hVertexErrorZ"); ?>
   <div class="thumbcaption">
   Vertex position error Z
   </div>

</table>


</div>
<!-- }}} -->



<h2 id="h_vertex_mc_reco_max_rank" class="count">Max Rank Reco vs Simu Vertex</h2>
<!-- {{{ -->
<div id="div:h_vertex_mc_reco_max_rank" class="section">


<p>
<table class="simple00 cntr">

<tr>
   <td><?php print $gP->img("vertex_maxrank/hVertexPullX"); ?>
   <div class="thumbcaption">
   Vertex pull distribuition in X
   </div>

   <td><?php print $gP->img("vertex_maxrank/hVertexPullY"); ?>
   <div class="thumbcaption">
   Vertex pull distribuition in Y
   </div>

   <td><?php print $gP->img("vertex_maxrank/hVertexPullZ"); ?>
   <div class="thumbcaption">
   Vertex pull distribuition in Z
   </div>

<tr>
   <td><?php print $gP->img("vertex_maxrank/hMcRecoVertexDeltaX"); ?>
   <div class="thumbcaption">
   Delta X: Simu - Reco
   </div>

   <td><?php print $gP->img("vertex_maxrank/hMcRecoVertexDeltaY"); ?>
   <div class="thumbcaption">
   Delta Y: Simu - Reco
   </div>

   <td><?php print $gP->img("vertex_maxrank/hMcRecoVertexDeltaZ"); ?>
   <div class="thumbcaption">
   Delta Z: Simu - Reco
   </div>

<tr>
   <td>&nbsp;

   <td><?php print $gP->img("vertex/hMcRecoVertexDelta"); ?>
   <div class="thumbcaption">
   Distance: Simu - Reco
   </div>

   <td>&nbsp;

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
