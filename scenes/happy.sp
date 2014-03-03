var play_happy () =
{

  // for(i in seq(0, 100)){
  //   for(toto in seq(0, 100)){
  //     //    var beta = beta0 + beta1 * (toto / 100.0);
  //     reset();
  //     img.drawmesh(walk_matthieu.get(toto % 12), 640, 400, - 660, -0.5, 5.4, 0, 700);
  //     stay(0.1);
  //     snapshot();
  //   }
  // }
  
  // rotate(v.get(1), - 250, 5.4, 20);
  // stay(1);
  // rotate(v.get(0), - 250, 5.4, 20);

  //  stay(10);
  
  img.clear();

  var show_video(directory) =
    {
      var files = list_files(directory);
      var ancre = loadimage("input/ancre2.gif");
      img.drawimage(ancre, 700, 0, 1, 1);
      var frame = 0;
      for(file in files)
	{
	  set frame = 1 + frame;
	  if(not(frame % speed)){
	    var image = loadimage(file);
	    var small = image.resize(-20, -20);
	    img.drawimage(small, 100, 200, 1, 0);
	    img.drawimage(small, 500, 200, 0.2, 0);
	    snapshot();
	  }
	  else;
	}
    }

  show_video("videoextracts/testvideoiphone");
}
