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
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_20_VFMinuit/k0short.lis_result">VFMinuit</a>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_20_VFMinuitBeamline/k0short.lis_result">VFMinuit</a>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_20_VFMinuitBeamline3D/k0short.lis_result">VFMinuit</a>

   <tr>
      <th>PPV
      <td>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_20_VFPPVnoCTBBeamline/k0short.lis_result">PPV</a>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_20_VFPPVnoCTBBeamline3D/k0short.lis_result">PPV</a>

   <tr>
      <th>KFVF
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_20_KFVertex/k0short.lis_result">KFVF</a>
      <td colspan=2> <a href="plots.php?pfx=public/DEV_sti_Jun_20_KFVertexBeamline/k0short.lis_result">KFVF</a>

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
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_20_VFMinuit/minbias.lis_result">VFMinuit</a>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_20_VFMinuitBeamline/minbias.lis_result">VFMinuit</a>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_20_VFMinuitBeamline3D/minbias.lis_result">VFMinuit</a>

   <tr>
      <th>PPV
      <td>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_20_VFPPVnoCTBBeamline/minbias.lis_result">PPV</a>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_20_VFPPVnoCTBBeamline3D/minbias.lis_result">PPV</a>

   <tr>
      <th>KFVF
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_20_KFVertex/minbias.lis_result">KFVF</a>
      <td colspan=2> <a href="plots.php?pfx=public/DEV_sti_Jun_20_KFVertexBeamline/minbias.lis_result">KFVF</a>

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
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_02_VFMinuit/dstar.lis_result">VFMinuit</a>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_02_VFMinuitBeamline/dstar.lis_result">VFMinuit</a>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_02_VFMinuitBeamline3D/dstar.lis_result">VFMinuit</a>

   <tr>
      <th>PPV
      <td>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_02_VFPPVnoCTBBeamline/dstar.lis_result">PPV</a>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_02_VFPPVnoCTBBeamline3D/dstar.lis_result">PPV</a>

   <tr>
      <th>KFVF
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_02_KFVertex/dstar.lis_result">KFVF</a>
      <td colspan=2> <a href="plots.php?pfx=public/DEV_sti_Jun_02_KFVertexBeamline/dstar.lis_result">KFVF</a>

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
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_02_VFMinuit/k0short.lis_result">VFMinuit</a>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_02_VFMinuitBeamline/k0short.lis_result">VFMinuit</a>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_02_VFMinuitBeamline3D/k0short.lis_result">VFMinuit</a>

   <tr>
      <th>PPV
      <td>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_02_VFPPVnoCTBBeamline/k0short.lis_result">PPV</a>
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_02_VFPPVnoCTBBeamline3D/k0short.lis_result">PPV</a>

   <tr>
      <th>KFVF
      <td> <a href="plots.php?pfx=public/DEV_sti_Jun_02_KFVertex/k0short.lis_result">KFVF</a>
      <td colspan=2> <a href="plots.php?pfx=public/DEV_sti_Jun_02_KFVertexBeamline/k0short.lis_result">KFVF</a>

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
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-minuit-beamlineno-xysigma-0.1_pions_n_016.lis_result">VFMinuit/0.1/16</a>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-minuit-beamline1D-xysigma-0.1_pions_n_016.lis_result">VFMinuit/0.1/16</a>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-minuit-beamline3D-xysigma-0.1_pions_n_016.lis_result">VFMinuit/0.1/16</a>

   <tr>

      <th>2 tracks
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-minuit-beamlineno-xysigma-0.1_pions_n_002.lis_result">VFMinuit/0.1/2</a>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-minuit-beamline1D-xysigma-0.1_pions_n_002.lis_result">VFMinuit/0.1/2</a>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-minuit-beamline3D-xysigma-0.1_pions_n_002.lis_result">VFMinuit/0.1/2</a>

   <tr>
      <th rowspan=2>PPV
      <th>16 tracks
      <td>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-ppv-beamline1D-xysigma-0.1_pions_n_016.lis_result">PPV/0.1/16</a>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-ppv-beamline3D-xysigma-0.1_pions_n_016.lis_result">PPV/0.1/16</a>

   <tr>

      <th>2 tracks
      <td>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-ppv-beamline1D-xysigma-0.1_pions_n_002.lis_result">PPV/0.1/2</a>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-ppv-beamline3D-xysigma-0.1_pions_n_002.lis_result">PPV/0.1/2</a>

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
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-minuit-beamlineno-xysigma-0.01_pions_n_016.lis_result">VFMinuit/0.01/16</a>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-minuit-beamline1D-xysigma-0.01_pions_n_016.lis_result">VFMinuit/0.01/16</a>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-minuit-beamline3D-xysigma-0.01_pions_n_016.lis_result">VFMinuit/0.01/16</a>

   <tr>

      <th>2 tracks
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-minuit-beamlineno-xysigma-0.01_pions_n_002.lis_result">VFMinuit/0.01/2</a>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-minuit-beamline1D-xysigma-0.01_pions_n_002.lis_result">VFMinuit/0.01/2</a>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-minuit-beamline3D-xysigma-0.01_pions_n_002.lis_result">VFMinuit/0.01/2</a>

   <tr>
      <th rowspan=2>PPV
      <th>16 tracks
      <td>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-ppv-beamline1D-xysigma-0.01_pions_n_016.lis_result">PPV/0.01/16</a>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-ppv-beamline3D-xysigma-0.01_pions_n_016.lis_result">PPV/0.01/16</a>

   <tr>

      <th>2 tracks
      <td>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-ppv-beamline1D-xysigma-0.01_pions_n_002.lis_result">PPV/0.01/2</a>
      <td> <a href="plots.php?pfx=public/cvs-160428-vertex-ppv-beamline3D-xysigma-0.01_pions_n_002.lis_result">PPV/0.01/2</a>

   </table>

</ul>


<?php
include_once(BASE_DIR."/common/bottom.php");
?>
