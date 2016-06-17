<?php

include_once("../common/config.php");

$page_title = "Vtxeval Results";

include_once(BASE_DIR."/common/head.php");

?>

<h1>Vtxeval Results</h1>


<h2 class="colsep">MinuitVF, beamline as track</h2>

<ul>
   <li>16 tracks:

   <br>beamline, beam width (cm):
   <a href="plots.php?pfx=a1bae679-vertex-minuit-fit-beamline-xysigma-0.1_pions_n_016">0.1</a>&nbsp;&nbsp;&nbsp;
   <a href="plots.php?pfx=a1bae679-vertex-minuit-fit-beamline-xysigma-0.01_pions_n_016">0.01</a>

   <br>no beamline, beam width (cm):
   <a href="plots.php?pfx=vertex-minuit-fit-xysigma-0.1-4cfe5e77_pions_n_016">0.1</a>&nbsp;&nbsp;&nbsp;
   <a href="plots.php?pfx=vertex-minuit-fit-xysigma-0.01-4cfe5e77_pions_n_016">0.01</a>


   <li>8 tracks:

   <br>beamline, beam width (cm):
   <a href="plots.php?pfx=a1bae679-vertex-minuit-fit-beamline-xysigma-0.1_pions_n_008">0.1</a>&nbsp;&nbsp;&nbsp;
   <a href="plots.php?pfx=a1bae679-vertex-minuit-fit-beamline-xysigma-0.01_pions_n_008">0.01</a>

   <br>no beamline, beam width (cm):
   <a href="plots.php?pfx=vertex-minuit-fit-xysigma-0.1-4cfe5e77_pions_n_008">0.1</a>&nbsp;&nbsp;&nbsp;
   <a href="plots.php?pfx=vertex-minuit-fit-xysigma-0.01-4cfe5e77_pions_n_008">0.01</a>


   <li>4 tracks,

   <br>beamline, beam width (cm):
   <a href="plots.php?pfx=a1bae679-vertex-minuit-fit-beamline-xysigma-0.1_pions_n_004">0.1</a>&nbsp;&nbsp;&nbsp;
   <a href="plots.php?pfx=a1bae679-vertex-minuit-fit-beamline-xysigma-0.01_pions_n_004">0.01</a>

   <br>no beamline, beam width (cm):
   <a href="plots.php?pfx=vertex-minuit-fit-xysigma-0.1-4cfe5e77_pions_n_004">0.1</a>&nbsp;&nbsp;&nbsp;
   <a href="plots.php?pfx=vertex-minuit-fit-xysigma-0.01-4cfe5e77_pions_n_004">0.01</a>


   <li>2 tracks

   <br>beamline, beam width (cm):
   <a href="plots.php?pfx=a1bae679-vertex-minuit-fit-beamline-xysigma-0.1_pions_n_002">0.1</a>&nbsp;&nbsp;&nbsp;
   <a href="plots.php?pfx=a1bae679-vertex-minuit-fit-beamline-xysigma-0.01_pions_n_002">0.01</a>

   <br>no beamline, beam width (cm):
   <a href="plots.php?pfx=vertex-minuit-fit-xysigma-0.1-4cfe5e77_pions_n_002">0.1</a>&nbsp;&nbsp;&nbsp;
   <a href="plots.php?pfx=vertex-minuit-fit-xysigma-0.01-4cfe5e77_pions_n_002">0.01</a>
</ul>


<h2 class="colsep">D<sup>0</sup> enhanced, HFT and TPC pile-up - by Liang</h2>

<ul>
   <li><a href="plots.php?pfx=vertex-ppv-fit-ccbar-sim">PPV + fit</a>
</ul>


<h2 class="colsep">W, no pile-up, no HFT hits</h2>

<ul>
   <li><a href="plots.php?pfx=vertex-ppv-5e4e0fc4">PPV</a>
   <li><a href="plots.php?pfx=vertex-ppv-fitter-50ff3250">PPV + fit</a>
   <li><a href="plots.php?pfx=vertex-minuit-5e4e0fc4">MinuitVF</a>
   <li><a href="plots.php?pfx=vertex-kfv-0fd24f22">KFV</a>
   <li><a href="plots.php?pfx=vertex-kfv-beam-err">KFV (with beamline error changed 0.01 -> 0.1 cm)</a>
</ul>



<?php
include_once(BASE_DIR."/common/bottom.php");
?>
