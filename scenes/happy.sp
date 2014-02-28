{
  var show_video(directory) =
    {
      var files = list_files(directory);
      for(file in files)
	{
	  var image = loadimage(file);
	  var small = image.resize(-20, -20);
	  drawimage(img, small, 100, 200, 1, 0);
	  drawimage(img, small, 500, 200, 0.5, 0);
	  snapshot();
	}
    }

  show_video("videoextracts/testvideoiphone");
}
