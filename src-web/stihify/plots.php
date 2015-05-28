<?php
include_once("../common/config.php");

$page_title = "Stihify Plots";

include_once(BASE_DIR."/common/head.php");
include_once(BASE_DIR."/common/PlotHelper.php");

?>


<h1>Plots</h1>

<?php
if (file_exists($info_file))
   readfile($info_file);
?>


<div id="main_text_body">
<!-- {{{ -->

</div> <!--main_text_body-->
<!-- }}} -->


<?php
include_once(BASE_DIR."/common/bottom.php");
?>
