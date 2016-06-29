<?php

include_once("../common/config.php");

$page_title = "Vtxeval Results";

include_once(BASE_DIR."/common/head.php");

?>

<h1>Vtxeval Results</h1>


<h2 class="colsep">pp, signal, pile-up</h2>

<ul>

   <li><b>K0, Beam width: 0.01 cm</b>

   <p>
   <table class="generic">
   <tr>
      <td>
      <th>no beamline
      <th>beamline (aka 1D fit)
      <th>beamline3D (aka 3D fit)

   <tr>
      <th>VFMinuit
      <td> <a href="plots.php?pfx=DEV_sti_Jun_20_VFMinuit/k0short">VFMinuit</a>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_20_VFMinuitBeamline/k0short">VFMinuit</a>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_20_VFMinuitBeamline3D/k0short">VFMinuit</a>

   <tr>
      <th>PPV
      <td>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_20_VFPPVnoCTBBeamline/k0short">PPV</a>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_20_VFPPVnoCTBBeamline3D/k0short">PPV</a>

   <tr>
      <th>KFVF
      <td> <a href="plots.php?pfx=DEV_sti_Jun_20_KFVertex/k0short">KFVF</a>
      <td colspan=2> <a href="plots.php?pfx=DEV_sti_Jun_20_KFVertexBeamline/k0short">KFVF</a>

   </table>

   <li><b>minbias, Beam width: 0.01 cm</b>

   <p>
   <table class="generic">
   <tr>
      <td>
      <th>no beamline
      <th>beamline (aka 1D fit)
      <th>beamline3D (aka 3D fit)

   <tr>
      <th>VFMinuit
      <td> <a href="plots.php?pfx=DEV_sti_Jun_20_VFMinuit/minbias">VFMinuit</a>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_20_VFMinuitBeamline/minbias">VFMinuit</a>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_20_VFMinuitBeamline3D/minbias">VFMinuit</a>

   <tr>
      <th>PPV
      <td>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_20_VFPPVnoCTBBeamline/minbias">PPV</a>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_20_VFPPVnoCTBBeamline3D/minbias">PPV</a>

   <tr>
      <th>KFVF
      <td> <a href="plots.php?pfx=DEV_sti_Jun_20_KFVertex/minbias">KFVF</a>
      <td colspan=2> <a href="plots.php?pfx=DEV_sti_Jun_20_KFVertexBeamline/minbias">KFVF</a>

   </table>

</ul>


<h2 class="colsep">pp with simulated pile-up</h2>

<ul>
   <li><b>D*, Beam width: 0.01 cm</b>

   <p>
   <table class="generic">
   <tr>
      <td>
      <th>no beamline
      <th>beamline (aka 1D fit)
      <th>beamline3D (aka 3D fit)

   <tr>
      <th>VFMinuit
      <td> <a href="plots.php?pfx=DEV_sti_Jun_02_VFMinuit/dstar">VFMinuit</a>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_02_VFMinuitBeamline/dstar">VFMinuit</a>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_02_VFMinuitBeamline3D/dstar">VFMinuit</a>

   <tr>
      <th>PPV
      <td>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_02_VFPPVnoCTBBeamline/dstar">PPV</a>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_02_VFPPVnoCTBBeamline3D/dstar">PPV</a>

   <tr>
      <th>KFVF
      <td> <a href="plots.php?pfx=DEV_sti_Jun_02_KFVertex/dstar">KFVF</a>
      <td colspan=2> <a href="plots.php?pfx=DEV_sti_Jun_02_KFVertexBeamline/dstar">KFVF</a>

   </table>

   <li><b>K0, Beam width: 0.01 cm</b>

   <p>
   <table class="generic">
   <tr>
      <td>
      <th>no beamline
      <th>beamline (aka 1D fit)
      <th>beamline3D (aka 3D fit)

   <tr>
      <th>VFMinuit
      <td> <a href="plots.php?pfx=DEV_sti_Jun_02_VFMinuit/k0short">VFMinuit</a>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_02_VFMinuitBeamline/k0short">VFMinuit</a>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_02_VFMinuitBeamline3D/k0short">VFMinuit</a>

   <tr>
      <th>PPV
      <td>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_02_VFPPVnoCTBBeamline/k0short">PPV</a>
      <td> <a href="plots.php?pfx=DEV_sti_Jun_02_VFPPVnoCTBBeamline3D/k0short">PPV</a>

   <tr>
      <th>KFVF
      <td> <a href="plots.php?pfx=DEV_sti_Jun_02_KFVertex/k0short">KFVF</a>
      <td colspan=2> <a href="plots.php?pfx=DEV_sti_Jun_02_KFVertexBeamline/k0short">KFVF</a>

   </table>

</ul>



<h2 class="colsep">STAR Vertex Finders with different beamline options</h2>

<ul>
   <li><b>Beam width: 0.1 cm</b>

   <p>
   <table class="generic">
   <tr>
      <td>
      <td>
      <th>no beamline
      <th>beamline (aka 1D fit)
      <th>beamline3D (aka 3D fit)

   <tr>
      <th rowspan=2>VFMinuit
      <th>16 tracks
      <td> <a href="plots.php?pfx=cvs-160428-vertex-minuit-beamlineno-xysigma-0.1_pions_n_016">VFMinuit/0.1/16</a>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-minuit-beamline1D-xysigma-0.1_pions_n_016">VFMinuit/0.1/16</a>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-minuit-beamline3D-xysigma-0.1_pions_n_016">VFMinuit/0.1/16</a>

   <tr>

      <th>2 tracks
      <td> <a href="plots.php?pfx=cvs-160428-vertex-minuit-beamlineno-xysigma-0.1_pions_n_002">VFMinuit/0.1/2</a>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-minuit-beamline1D-xysigma-0.1_pions_n_002">VFMinuit/0.1/2</a>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-minuit-beamline3D-xysigma-0.1_pions_n_002">VFMinuit/0.1/2</a>

   <tr>
      <th rowspan=2>PPV
      <th>16 tracks
      <td>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-ppv-beamline1D-xysigma-0.1_pions_n_016">PPV/0.1/16</a>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-ppv-beamline3D-xysigma-0.1_pions_n_016">PPV/0.1/16</a>

   <tr>

      <th>2 tracks
      <td>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-ppv-beamline1D-xysigma-0.1_pions_n_002">PPV/0.1/2</a>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-ppv-beamline3D-xysigma-0.1_pions_n_002">PPV/0.1/2</a>

   </table>


   <li><b>Beam width: 0.01 cm</b>

   <p>
   <table class="generic">
   <tr>
      <td>
      <td>
      <th>no beamline
      <th>beamline (aka 1D fit)
      <th>beamline3D (aka 3D fit)

   <tr>
      <th rowspan=2>VFMinuit
      <th>16 tracks
      <td> <a href="plots.php?pfx=cvs-160428-vertex-minuit-beamlineno-xysigma-0.01_pions_n_016">VFMinuit/0.01/16</a>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-minuit-beamline1D-xysigma-0.01_pions_n_016">VFMinuit/0.01/16</a>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-minuit-beamline3D-xysigma-0.01_pions_n_016">VFMinuit/0.01/16</a>

   <tr>

      <th>2 tracks
      <td> <a href="plots.php?pfx=cvs-160428-vertex-minuit-beamlineno-xysigma-0.01_pions_n_002">VFMinuit/0.01/2</a>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-minuit-beamline1D-xysigma-0.01_pions_n_002">VFMinuit/0.01/2</a>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-minuit-beamline3D-xysigma-0.01_pions_n_002">VFMinuit/0.01/2</a>

   <tr>
      <th rowspan=2>PPV
      <th>16 tracks
      <td>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-ppv-beamline1D-xysigma-0.01_pions_n_016">PPV/0.01/16</a>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-ppv-beamline3D-xysigma-0.01_pions_n_016">PPV/0.01/16</a>

   <tr>

      <th>2 tracks
      <td>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-ppv-beamline1D-xysigma-0.01_pions_n_002">PPV/0.01/2</a>
      <td> <a href="plots.php?pfx=cvs-160428-vertex-ppv-beamline3D-xysigma-0.01_pions_n_002">PPV/0.01/2</a>

   </table>

</ul>


<?php
include_once(BASE_DIR."/common/bottom.php");
?>
