<?php

include_once("config.php");


class PlotHelper
{
   private $img_base_dir;
   private $img_name_prefix;
   private $img_width;
   private $img_extension;


   function __construct($img_base_dir, $img_name_prefix="c_", $img_width=500, $img_extension=".png")
   {
      $this->img_width = $img_width;
      $this->img_base_dir = $img_base_dir;
      $this->img_name_prefix = $img_name_prefix;
      $this->img_extension = $img_extension;
   }


   function fileExists($pName)
   {
      $pi = pathinfo($pName);
      $pfx = $this->img_name_prefix;
      $ext = $this->img_extension;

      $imgSrc  = STISCAN_RESULTS_DIR."/{$pi['dirname']}/$pfx{$pi['basename']}$ext";
      return file_exists($imgSrc);
   }


   function img($pName, $w=null, $href=null)
   {
      $pi = pathinfo($pName);
      $pfx = $this->img_name_prefix;
      $ext = $this->img_extension;

      $strWidth = empty($w) ? "width={$this->img_width}" : "width=$w";
      $imgSrc   = "{$this->img_base_dir}/{$pi['dirname']}/$pfx{$pi['basename']}$ext";
      $imgHref  = empty($href) ? "{$this->img_base_dir}/{$pi['dirname']}/$pfx{$pi['basename']}$ext" : $href;

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
