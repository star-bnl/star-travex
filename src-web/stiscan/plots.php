<?php

include_once("../common/config.php");

$page_title = "Stiscan Plots";

include_once(BASE_DIR."/common/head.php");
include_once(BASE_DIR."/common/PlotHelper.php");


$pfx = isset($_GET['pfx']) ? $_GET['pfx'] : "";

$info_file = STISCAN_RESULTS_DIR."/".dirname($pfx)."/info.html";
$gP = new PlotHelper(STISCAN_RESULTS_URL."/$pfx");

?>


<h1>Plots</h1>

<?php
if (file_exists($info_file))
   readfile($info_file);
?>


<div id="main_text_body">
<!-- {{{ -->


<h1 id="r_vs_phi" class="count">r vs &phi;</h1>
<!-- {{{ -->
<div id="div:r_vs_phi" class="section">


<h2 id="r_vs_phi_eloss" class="count">Energy Losses</h2>
<!-- {{{ -->
<div id="div:r_vs_phi_eloss" class="section">

<p>
<table class="simple00 cntr">

<tr>
   <th>Geant Simulation
   <th>Sti Reconstruction
   <th>Relative Ratio: (Sti-Geant)/Geant

<tr>
   <td><?php print $gP->img("gea/hELossVsXVsYVsZ_pyx", 800); ?>
   <td><?php print $gP->img("sti_trk/hELossVsXVsYVsZ_pyx", 800); ?>
   <td><?php print $gP->img("sti_gea_ratio/hELossVsXVsYVsZ_pyx", 800); ?>

<tr>
   <td><?php print $gP->img("gea/hELossVsPhiVsRVsZ_pyx", 800); ?>
   <td><?php print $gP->img("sti_trk/hELossVsPhiVsRVsZ_pyx", 800); ?>
   <td><?php print $gP->img("sti_gea_ratio/hELossVsPhiVsRVsZ_pyx", 800); ?>

<tr>
   <td><?php print $gP->img("gea/hELossVsPhiVsRVsZ_pyx_px", 800); ?>
   <td><?php print $gP->img("sti_trk/hELossVsPhiVsRVsZ_pyx_px", 800); ?>
   <td><?php print $gP->img("sti_gea_ratio/hELossVsPhiVsRVsZ_pyx_px", 800); ?>

</table>

</div>
<!-- }}} -->


<h2 id="r_vs_phi_nsteps" class="count">Average Number of Steps</h2>
<!-- {{{ -->
<div id="div:r_vs_phi_nsteps" class="section">

<p>
<table class="simple00 cntr">

<tr>
   <th>Geant Simulation
   <th>Sti Reconstruction

<tr>
   <td><?php print $gP->img("gea/hNStepsVsPhiVsRVsZ_pyx", 800); ?>
   <td><?php print $gP->img("sti_trk/hNStepsVsPhiVsRVsZ_pyx", 800); ?>

<tr>
   <td><?php print $gP->img("gea/hNStepsVsPhiVsRVsZ_pyx_px", 800); ?>
   <td><?php print $gP->img("sti_trk/hNStepsVsPhiVsRVsZ_pyx_px", 800); ?>

</table>

</div>
<!-- }}} -->


<h2 id="r_vs_phi_radlen" class="count">Relative Radiation Length</h2>
<!-- {{{ -->
<div id="div:r_vs_phi_radlen" class="section">

<p>
<table class="simple00 cntr">

<tr>
   <th>Geant Simulation
   <th>Sti Reconstruction

<tr>
   <td><?php print $gP->img("gea/hRelRadLengthVsPhiVsRVsZ_pyx", 800); ?>
   <td><?php print $gP->img("sti_trk/hRelRadLengthVsPhiVsRVsZ_pyx", 800); ?>

<tr>
   <td><?php print $gP->img("gea/hRelRadLengthVsPhiVsRVsZ_pyx_px", 800); ?>
   <td><?php print $gP->img("sti_trk/hRelRadLengthVsPhiVsRVsZ_pyx_px", 800); ?>

</table>

</div>
<!-- }}} -->


<h2 id="r_vs_phi_dens" class="count">Material Density</h2>
<!-- {{{ -->
<div id="div:r_vs_phi_dens" class="section">

<p>
<table class="simple00 cntr">

<tr>
   <th>Geant Simulation
   <th>Sti Reconstruction

<?php
if (!$gP->fileExists("$pfx/gea/hDensityVsPhiVsRVsZ_pyx")) :
?>

<tr>
   <td><?php print $gP->img("gea/hDensityVsPhiVsR", 800); ?>
   <td><?php print $gP->img("sti_trk/hDensityVsPhiVsR", 800); ?>

<tr>
   <td><?php print $gP->img("gea/hDensityVsPhiVsR_px", 800); ?>
   <td><?php print $gP->img("sti_trk/hDensityVsPhiVsR_px", 800); ?>

<?php
else:
?>

<tr>
   <td><?php print $gP->img("gea/hDensityVsPhiVsRVsZ_pyx", 800); ?>
   <td><?php print $gP->img("sti_trk/hDensityVsPhiVsRVsZ_pyx", 800); ?>

<tr>
   <td><?php print $gP->img("gea/hDensityVsPhiVsRVsZ_pyx_px", 800); ?>
   <td><?php print $gP->img("sti_trk/hDensityVsPhiVsRVsZ_pyx_px", 800); ?>

<?php
endif;
?>

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
