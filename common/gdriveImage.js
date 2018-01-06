
var updateImages = function() {
  $('img.img_topdir').each( function() {
      var $img = $(this);
      var img_new_src = "https://drive.google.com/uc?id=" + gdrive_file_map[ $img.attr('src') ]; ;
      $img.attr('src', img_new_src);
      $img.wrap('<a href="' + img_new_src + '"></a>');
  });
};


function updateGDriveImages(urlParams)
{
  // Adding the script tag to the head
  var head = document.getElementsByTagName('head')[0];
  var script = document.createElement('script');
  script.type = 'text/javascript';
  script.src = 'https://script.google.com/macros/s/AKfycbxhd0vLTV37YHWm3T7JWGQ-vM0rIQsfBUAGiqi8mHoDAAXETu1i/dev?pfx=public/' + urlParams.pfx;
  
  // Then bind the event to the callback function.
  // There are several events for cross browser compatibility.
  script.onreadystatechange = updateImages;
  script.onload = updateImages;
  
  // Fire the loading
  head.appendChild(script);
}
