function plotHistsFromUrlParams()
{
  var query = window.location.search.substring(1);
  var urlParams = $.deparam(query, true);

  var path = urlParams.path ? urlParams.path : "";
  var files = urlParams.files;

  plotHists(path, files);
}


function plotHists(path, files)
{
  var thstacks = {};

  $('div.canvas_jsroot').each( function() {
    var div = $(this);
    var obj_name = div.attr('id');
    var div_name = div.attr('id').replace(/\//g, "\\/");

    thstacks[obj_name] = JSROOT.Create("THStack");
  });

  JSROOT.gStyle.fPadGridX = true;
  JSROOT.gStyle.fPadGridY = true;
  JSROOT.gStyle.fPadBottomMargin = 0.1;
  JSROOT.gStyle.fPadRightMargin = 0.1;
  JSROOT.gStyle.fStatStyle = 0;
  JSROOT.gStyle.fStatFontSize = 0.5;
  JSROOT.gStyle.fStatX = 1;
  JSROOT.gStyle.fStatY = 1;
  JSROOT.gStyle.fStatW = 0.25;
  JSROOT.gStyle.fStatH = 0.30;


  for (var i = 0; i < files.length; i++) {

    var filename = path + files[i];

    (function(findex) {

      JSROOT.OpenFile(filename, function(file) {

        $('div.canvas_jsroot').each( function() {

          var $div = $(this);
          var obj_name = $div.attr('id');
          var div_name = $div.attr('id').replace(/\//g, "\\/");

          file.ReadObject(obj_name, function(obj) {

            if (obj == null) return;

            obj.fLineColor = findex + 1;
            obj.fLineWidth = 2;

            thstacks[obj_name].fHists.Add(obj);

            JSROOT.redraw(div_name, thstacks[obj_name], "nostack");
          });
        });
      });
    })(i);
  }
};
