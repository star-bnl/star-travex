<?php

include_once("../common/config.php");

$page_title = "Vtxeval Plots";

include_once(BASE_DIR."/common/head.php");


$pfx = isset($_GET['pfx']) ? $_GET['pfx'] : "";
$det = isset($_GET['det']) ? $_GET['det'] : "";

$info_file = VTXEVAL_RESULTS_DIR."/".dirname($pfx)."/info.html";

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
   <td><img class="img_topdir" width=500 src="event/hNumTracksPerEvent.png">
   <div class="thumbcaption">
   Total number of tracks in event
   </div>

   <td><img class="img_topdir" width=500 src="event/hNumTracksPerEventPrimary.png">
   <div class="thumbcaption">
   Total number of primary tracks in event
   </div>

   <td><img class="img_topdir" width=500 src="event/hNumVertices.png">
   <div class="thumbcaption">
   Total number of vertices
   </div>

<tr>
   <td>&nbsp;

   <td><img class="img_topdir" width=500 src="event/McRecMulAny_eff.png">
   <div class="thumbcaption">
   Primary vertex reconstruction efficiency
   </div>

   <td><img class="img_topdir" width=500 src="event/McRecMulGood_eff.png">
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
   <td><img class="img_topdir" width=500 src="vertex/hMcVertexX.png">
   <div class="thumbcaption">
   Vertex position X
   </div>

   <td>&nbsp;

   <td><img class="img_topdir" width=500 src="vertex/hMcVertexZvX.png">
   <div class="thumbcaption">
   Vertex position Z vs X
   </div>

<tr>

   <td><img class="img_topdir" width=500 src="vertex/hMcVertexXvY.png">
   <div class="thumbcaption">
   Vertex position X vs Y
   </div>

   <td><img class="img_topdir" width=500 src="vertex/hMcVertexY.png">
   <div class="thumbcaption">
   Vertex position Y
   </div>

   <td><img class="img_topdir" width=500 src="vertex/hMcVertexZvY.png">
   <div class="thumbcaption">
   Vertex position Z vs Y
   </div>

<tr>
   <td>&nbsp;

   <td>&nbsp;

   <td><img class="img_topdir" width=500 src="vertex/hMcVertexZ.png">
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
   <td><img class="img_topdir" width=500 src="vertex/hVertexX.png">
   <div class="thumbcaption">
   Vertex position X
   </div>

   <td>&nbsp;

   <td><img class="img_topdir" width=500 src="vertex/hVertexZvX.png">
   <div class="thumbcaption">
   Vertex position Z vs X
   </div>

<tr>

   <td><img class="img_topdir" width=500 src="vertex/hVertexXvY.png">
   <div class="thumbcaption">
   Vertex position X vs Y
   </div>

   <td><img class="img_topdir" width=500 src="vertex/hVertexY.png">
   <div class="thumbcaption">
   Vertex position Y
   </div>

   <td><img class="img_topdir" width=500 src="vertex/hVertexZvY.png">
   <div class="thumbcaption">
   Vertex position Z vs Y
   </div>

<tr>
   <td>&nbsp;

   <td>&nbsp;

   <td><img class="img_topdir" width=500 src="vertex/hVertexZ.png">
   <div class="thumbcaption">
   Vertex position Z
   </div>

<tr>
   <td><img class="img_topdir" width=500 src="vertex/hVertexErrorX.png">
   <div class="thumbcaption">
   Vertex position error X
   </div>

   <td><img class="img_topdir" width=500 src="vertex/hVertexErrorY.png">
   <div class="thumbcaption">
   Vertex position error Y
   </div>

   <td><img class="img_topdir" width=500 src="vertex/hVertexErrorZ.png">
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
   <td><img class="img_topdir" width=500 src="vertex_hft/hVertexX.png">
   <div class="thumbcaption">
   Vertex position X
   </div>

   <td>&nbsp;

   <td><img class="img_topdir" width=500 src="vertex_hft/hVertexZvX.png">
   <div class="thumbcaption">
   Vertex position Z vs X
   </div>

<tr>

   <td><img class="img_topdir" width=500 src="vertex_hft/hVertexXvY.png">
   <div class="thumbcaption">
   Vertex position X vs Y
   </div>

   <td><img class="img_topdir" width=500 src="vertex_hft/hVertexY.png">
   <div class="thumbcaption">
   Vertex position Y
   </div>

   <td><img class="img_topdir" width=500 src="vertex_hft/hVertexZvY.png">
   <div class="thumbcaption">
   Vertex position Z vs Y
   </div>

<tr>
   <td>&nbsp;

   <td>&nbsp;

   <td><img class="img_topdir" width=500 src="vertex_hft/hVertexZ.png">
   <div class="thumbcaption">
   Vertex position Z
   </div>

<tr>
   <td><img class="img_topdir" width=500 src="vertex_hft/hVertexErrorX.png">
   <div class="thumbcaption">
   Vertex position error X
   </div>

   <td><img class="img_topdir" width=500 src="vertex_hft/hVertexErrorY.png">
   <div class="thumbcaption">
   Vertex position error Y
   </div>

   <td><img class="img_topdir" width=500 src="vertex_hft/hVertexErrorZ.png">
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
   <td><img class="img_topdir" width=500 src="vertex/hVertexPullX.png">
   <div class="thumbcaption">
   Vertex pull distribuition in X
   </div>

   <td><img class="img_topdir" width=500 src="vertex/hVertexPullY.png">
   <div class="thumbcaption">
   Vertex pull distribuition in Y
   </div>

   <td><img class="img_topdir" width=500 src="vertex/hVertexPullZ.png">
   <div class="thumbcaption">
   Vertex pull distribuition in Z
   </div>

<tr>
   <td><img class="img_topdir" width=500 src="vertex/hMcRecoVertexDeltaX.png">
   <div class="thumbcaption">
   Delta X: Simu - Reco
   </div>

   <td><img class="img_topdir" width=500 src="vertex/hMcRecoVertexDeltaY.png">
   <div class="thumbcaption">
   Delta Y: Simu - Reco
   </div>

   <td><img class="img_topdir" width=500 src="vertex/hMcRecoVertexDeltaZ.png">
   <div class="thumbcaption">
   Delta Z: Simu - Reco
   </div>

<tr>
   <td>&nbsp;

   <td><img class="img_topdir" width=500 src="vertex/hMcRecoVertexDelta.png">
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
   <td><img class="img_topdir" width=500 src="vertex_hft/hVertexPullX.png">
   <div class="thumbcaption">
   Vertex pull distribuition in X
   </div>

   <td><img class="img_topdir" width=500 src="vertex_hft/hVertexPullY.png">
   <div class="thumbcaption">
   Vertex pull distribuition in Y
   </div>

   <td><img class="img_topdir" width=500 src="vertex_hft/hVertexPullZ.png">
   <div class="thumbcaption">
   Vertex pull distribuition in Z
   </div>

<tr>
   <td><img class="img_topdir" width=500 src="vertex_hft/hMcRecoVertexDeltaX.png">
   <div class="thumbcaption">
   Delta X: Simu - Reco
   </div>

   <td><img class="img_topdir" width=500 src="vertex_hft/hMcRecoVertexDeltaY.png">
   <div class="thumbcaption">
   Delta Y: Simu - Reco
   </div>

   <td><img class="img_topdir" width=500 src="vertex_hft/hMcRecoVertexDeltaZ.png">
   <div class="thumbcaption">
   Delta Z: Simu - Reco
   </div>

<tr>
   <td>&nbsp;

   <td><img class="img_topdir" width=500 src="vertex_hft/hMcRecoVertexDelta.png">
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
   <td><img class="img_topdir" width=500 src="vertex_maxrank/hVertexX.png">
   <div class="thumbcaption">
   Vertex position X
   </div>

   <td>&nbsp;

   <td><img class="img_topdir" width=500 src="vertex_maxrank/hVertexZvX.png">
   <div class="thumbcaption">
   Vertex position Z vs X
   </div>

<tr>

   <td><img class="img_topdir" width=500 src="vertex_maxrank/hVertexXvY.png">
   <div class="thumbcaption">
   Vertex position X vs Y
   </div>

   <td><img class="img_topdir" width=500 src="vertex_maxrank/hVertexY.png">
   <div class="thumbcaption">
   Vertex position Y
   </div>

   <td><img class="img_topdir" width=500 src="vertex_maxrank/hVertexZvY.png">
   <div class="thumbcaption">
   Vertex position Z vs Y
   </div>

<tr>
   <td>&nbsp;

   <td>&nbsp;

   <td><img class="img_topdir" width=500 src="vertex_maxrank/hVertexZ.png">
   <div class="thumbcaption">
   Vertex position Z
   </div>

<tr>
   <td><img class="img_topdir" width=500 src="vertex_maxrank/hVertexErrorX.png">
   <div class="thumbcaption">
   Vertex position error X
   </div>

   <td><img class="img_topdir" width=500 src="vertex_maxrank/hVertexErrorY.png">
   <div class="thumbcaption">
   Vertex position error Y
   </div>

   <td><img class="img_topdir" width=500 src="vertex_maxrank/hVertexErrorZ.png">
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
   <td><img class="img_topdir" width=500 src="vertex_maxrank/hVertexPullX.png">
   <div class="thumbcaption">
   Vertex pull distribuition in X
   </div>

   <td><img class="img_topdir" width=500 src="vertex_maxrank/hVertexPullY.png">
   <div class="thumbcaption">
   Vertex pull distribuition in Y
   </div>

   <td><img class="img_topdir" width=500 src="vertex_maxrank/hVertexPullZ.png">
   <div class="thumbcaption">
   Vertex pull distribuition in Z
   </div>

<tr>
   <td><img class="img_topdir" width=500 src="vertex_maxrank/hMcRecoVertexDeltaX.png">
   <div class="thumbcaption">
   Delta X: Simu - Reco
   </div>

   <td><img class="img_topdir" width=500 src="vertex_maxrank/hMcRecoVertexDeltaY.png">
   <div class="thumbcaption">
   Delta Y: Simu - Reco
   </div>

   <td><img class="img_topdir" width=500 src="vertex_maxrank/hMcRecoVertexDeltaZ.png">
   <div class="thumbcaption">
   Delta Z: Simu - Reco
   </div>

<tr>
   <td>&nbsp;

   <td><img class="img_topdir" width=500 src="vertex/hMcRecoVertexDelta.png">
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
