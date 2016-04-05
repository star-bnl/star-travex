<?php

include_once("../common/config.php");

$page_title = "Vtxeval Results";

include_once(BASE_DIR."/common/head.php");

?>

<h1>Vtxeval Results</h1>


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
