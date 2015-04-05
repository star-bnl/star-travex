<?php

include_once("config.php");


class PlotHelper
{
   var $img_base_dir;


   function PlotHelper($img_base_dir) {
      $this->img_base_dir = $img_base_dir;
   }


	function fileExists($pName)
	{
	   $pi = pathinfo($pName);
	   $imgSrc  = STISCAN_RESULTS_DIR."/{$pi['dirname']}/c_{$pi['basename']}.png";
		return file_exists($imgSrc);
	}


   function img($pName, $w=null, $href=null)
   {
	   $pi = pathinfo($pName);

      $strWidth = empty($w) ? "width=300" : "width=$w";
	   $imgSrc   = "{$this->img_base_dir}/{$pi['dirname']}/c_{$pi['basename']}.png";
	   $imgHref  = empty($href) ? "{$this->img_base_dir}/{$pi['dirname']}/c_{$pi['basename']}.png" : $href;

		return "<a href='$imgHref'><img $strWidth src='$imgSrc'></a>";
   }


   function htmlLinkHistContents($pName)
   {
	   $pi = pathinfo($pName);

	   $href = "{$this->img_base_dir}/{$pName}";

      return "<a href='$href'>{$pi['basename']}</a>";
   }
}

?>
