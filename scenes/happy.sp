var play_happy () =
{
  var balls = newmetametaball();
  var pt_plexus = newpoint(0, -10, 0);
  var pt_neck = pt_plexus.link(0, -4, 0);
  var pt_head = pt_neck.link(0, -1, 0);
  var pt_headbot = pt_head.link(0, 0, 1);
  var pt_thigh = pt_plexus.link(0, 5, 0);

  var pt_leftshoulder = pt_plexus.link(-2, 1.5, 0);
  var pt_leftelbow = pt_leftshoulder.link(-2, 2, 0);
  var pt_leftarm = pt_leftelbow.link(-2, 2, 0);

  var pt_rightshoulder = pt_plexus.link(2, 1.5, 0);
  var pt_rightelbow = pt_rightshoulder.link(2, 2, 0);
  var pt_rightarm = pt_rightelbow.link(2, 2, 0);

  var pt_lefthip = pt_thigh.link(-1, 1, 0);
  var pt_leftknee = pt_lefthip.link(-1, 4, 0);
  var pt_lefttoe = pt_leftknee.link(-1, 4, 0);

  var pt_righthip = pt_thigh.link(1, 1, 0);
  var pt_rightknee = pt_righthip.link(1, 4, 0);
  var pt_righttoe = pt_rightknee.link(1, 4, 0);

  balls.add(1, 10.5, grey(200), pt_head, pt_neck, pt_plexus);
  balls.add(1, 10.5, black, pt_headbot);
  balls.add(1, 10.5, brown, pt_neck, pt_plexus, pt_leftshoulder, pt_leftelbow, pt_leftarm);
  balls.add(1, 10.5, brown, pt_neck, pt_plexus, pt_rightshoulder, pt_rightelbow, pt_rightarm);
  balls.add(1.5, 10.5, red, pt_plexus, pt_thigh, pt_leftshoulder, pt_rightshoulder);
  balls.add(1.5, 10.5, blue, pt_plexus, pt_thigh, pt_lefthip, pt_leftknee);
  balls.add(1.5, 10.5, blue, pt_lefthip, pt_leftknee, pt_lefttoe);
  balls.add(1.5, 10.5, blue, pt_plexus, pt_thigh, pt_righthip, pt_rightknee);
  balls.add(1.5, 10.5, blue, pt_righthip, pt_rightknee, pt_righttoe);

  var mesh = balls.mesh(0.5, 25, 1000); 
  rotate(mesh, - 250, 5.4, 20);

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
