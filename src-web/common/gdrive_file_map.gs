function doGet(request)
{
  var userPath = request.parameter.pfx;
  var innermostFolderObj = getInnermostFolder(userPath);

  Logger.log( "innermostFolderObj: " + innermostFolderObj );

  if ( !innermostFolderObj )
  {
    var output = "ERROR: Innermost folder NOT found in path: " + userPath;
    Logger.log(output);
    return ContentService.createTextOutput(output);
  }

  var gdriveFile = provideFileMap(innermostFolderObj);
  var fileMapObj = gdriveFile.getAs('application/json');

  Logger.log( "fileMapObj size in bytes: " + fileMapObj.getBytes().length );

  var output = 'var gdrive_file_map = ' + fileMapObj.getDataAsString() + ';';

  return ContentService.createTextOutput(output).setMimeType(ContentService.MimeType.JAVASCRIPT);
}


function test_doGet()
{
  //var userPath = "/public/DEV_sti_Jun_02_KFVertex";
  //var userPath = "/public";
  var userPath = "public/4cfe5e77-vertex-minuit-fit-xysigma-0.1_pions_n_002.lis_result";

  var request = { parameter: { pfx: userPath } };

  var output = doGet(request);

  Logger.log( "doGet output:\n" + output.getContent() );
}


/**
* The path is assumed to start from the DriveApp root and include at least one sub folder name.
*/
function getInnermostFolder(path)
{
  var folders = path.split('/');

  // Remove empty elements
  var len = folders.length, i;
  // copy non-empty values to the end of the array
  for (i = 0; i < len; i++) folders[i] && folders.push(folders[i]);
  // cut the array and leave only the non-empty values
  folders.splice(0 , len);

  Logger.log( "folders: " + folders );

  var innermostFolderObj = folders.length > 0 ? DriveApp.getRootFolder() : null;

  for (var i = 0; i < folders.length; i++)
  {
    var subFolderName = folders[i];
    var subFolderIter = innermostFolderObj.getFoldersByName( subFolderName );

    Logger.log("Considering sub folder '" + subFolderName + "' in '" + innermostFolderObj.getName() + "'");

    if ( !subFolderIter.hasNext() ) {
      Logger.log( "ERROR: Folder '" + subFolderName + "' NOT found in '" + innermostFolderObj.getName() + "'");
      innermostFolderObj = null;
      break;
    }

    innermostFolderObj = subFolderIter.next();

    Logger.log("Setting innermost folder to '" + innermostFolderObj.getName() + "'");
  }

  return innermostFolderObj;
}


function provideFileMap(folderObj, useExisting)
{
  useExisting = typeof useExisting !== 'undefined' ? useExisting : true;

  var outputFileName = "gdrive_file_map.json";
  var gdriveFileIter = folderObj.getFilesByName( outputFileName );
  var gdriveFile = {};

  // Generate the map
  if ( gdriveFileIter.hasNext() && useExisting )
  {
    gdriveFile = gdriveFileIter.next();
    Logger.log("Reusing existing file: " + gdriveFile.getName() );
  } else
  {
    var parentFolders = [];
    var fileMap = {};

    traverseFolder(folderObj, parentFolders, fileMap);

    var fileMapContents = JSON.stringify(fileMap, null, 2);

    if (gdriveFileIter.hasNext())
    {
      gdriveFile = gdriveFileIter.next();
      gdriveFile.setContent( fileMapContents );
    } else
    {
      gdriveFile = folderObj.createFile( outputFileName, fileMapContents );
    }
  }

  //var fileUrl = 'https://drivenotepad.github.io/app/?state={"action":"open","ids":["' + gdriveFile.getId() + '"]}';
  var fileUrl = 'https://drive.google.com/uc?id=' + gdriveFile.getId();

  Logger.log( fileUrl );

  return gdriveFile;
}


function traverseFolder(folderObj, parentFolders, fileMap)
{
  var path = parentFolders.join('/');
  var files = folderObj.getFiles();

  Logger.log("dir: " + path);

  while (files.hasNext())
  {
    var file = files.next();
    fileMap[path + "/" + file.getName()] = file.getId();
  }

  var subFolders = folderObj.getFolders();

  while (subFolders.hasNext())
  {
    var subFolder = subFolders.next();
    parentFolders.push(subFolder.getName());
    traverseFolder(subFolder, parentFolders, fileMap);
    parentFolders.pop();
  }
}
