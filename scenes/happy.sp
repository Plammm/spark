var play_happy () =
{
  var balls = newmetametaball();
  var pt_plexus = newpoint(0, -10, 0);
  var pt_neck = pt_plexus.linkPoint(0, -4, 0);
  var pt_head = pt_neck.linkPoint(0, -1, 0);
  var pt_thigh = pt_plexus.linkPoint(0, 5, 0);

  var pt_leftshoulder = pt_plexus.linkPoint(-2, 1.5, 0);
  var pt_leftelbow = pt_leftshoulder.linkPoint(-2, 2, 0);
  var pt_leftarm = pt_leftelbow.linkPoint(-2, 2, 0);

  var pt_rightshoulder = pt_plexus.linkPoint(2, 1.5, 0);
  var pt_rightelbow = pt_rightshoulder.linkPoint(2, 2, 0);
  var pt_rightarm = pt_rightelbow.linkPoint(2, 2, 0);

  var pt_lefthip = pt_thigh.linkPoint(-1, 1, 0);
  var pt_leftknee = pt_lefthip.linkPoint(-1, 4, 0);
  var pt_lefttoe = pt_leftknee.linkPoint(-1, 4, 0);

  var pt_righthip = pt_thigh.linkPoint(1, 1, 0);
  var pt_rightknee = pt_righthip.linkPoint(1, 4, 0);
  var pt_righttoe = pt_rightknee.linkPoint(1, 4, 0);
  
  balls.newmetaball(1, 10.5, green, pt_head, pt_neck, pt_plexus);
  balls.newmetaball(1, 10.5, green, pt_neck, pt_plexus, pt_leftshoulder, pt_leftelbow, pt_leftarm);
  balls.newmetaball(1, 10.5, green, pt_neck, pt_plexus, pt_rightshoulder, pt_rightelbow, pt_rightarm);

  balls.newmetaball(1.5, 10.5, red, pt_plexus, pt_thigh, pt_leftshoulder, pt_rightshoulder);

  balls.newmetaball(1.5, 10.5, blue, pt_plexus, pt_thigh, pt_lefthip, pt_leftknee);
  balls.newmetaball(1.5, 10.5, blue, pt_lefthip, pt_leftknee, pt_lefttoe);

  //  balls.newmetaball(1.5, 10.5, blue, pt_plexus, pt_thigh, pt_rightshoulder, pt_rightshoulder);
  balls.newmetaball(1.5, 10.5, blue, pt_plexus, pt_thigh, pt_righthip, pt_rightknee);
  balls.newmetaball(1.5, 10.5, blue, pt_righthip, pt_rightknee, pt_righttoe);

  // var balls = newmetaball();

  // var radius = 1.4;
  // var threshold = 1.5;

  // var point(x, y, z, radius, threshold) = balls.addball(x, y, z, radius, threshold);
  // var line(x1, y1, z1, x2, y2, z2, n, radius, threshold) =
  //   {
  //     for(i in seq(0, n)){
  // 	var lambda = i / (0.0 + n);
  // 	point((1 - lambda) * x1 + lambda * x2,
  // 	      (1 - lambda) * y1 + lambda * y2,
  // 	      (1 - lambda) * z1 + lambda * z2, radius, threshold);
  //     }
  //   }

  // balls.addball(0, -12, 0, 4, 2);

  // //line(0,-8,0,4,-4,0,3, radius, threshold);
  // line(0,-8,0, -4,5,0, 15, 1, 1.1);
  // line(0,-8,0, 4,5,0, 15, 1, 1.1);
  // line(0,0,0,0,-10,0,10, radius, threshold);
  // line(0,0,0,-3,20,0,15, radius, threshold);
  // line(0,0,0,3,20,0,15, radius, threshold);

  // balls.addball(1, 0, 0, radius, threshold);
  // balls.addball(0.5, 1.5, 0, radius, threshold);
  // balls.addball(0, 3, 0, radius, threshold);
  // balls.addball(0, 4.5, 0, radius, threshold);
  // balls.addball(0, 4.5, 1, radius, threshold);

  // balls.addball(3, 0, 0, radius, threshold);
  // balls.addball(3.5, 1.5, 0, radius, threshold);
  // balls.addball(4, 3, 0, radius, threshold);
  // balls.addball(4, 4.5, 0, radius, threshold);
  // balls.addball(4, 4.5, 1, radius, threshold);

  // balls.addball(0.5, 0, 0, radius, threshold);
  // balls.addball(1, 0, 0, radius, threshold);
  // balls.addball(1.5, 0, 0, radius, threshold);
  // balls.addball(2, 0, 0, radius, threshold);
  // balls.addball(2.5, 0, 0, radius, threshold);
  // balls.addball(3, 0, 0, radius, threshold);
  // balls.addball(3.5, 0, 0, radius, threshold);

  // balls.addball(2, -1, 0, 2 * radius, threshold);
  // balls.addball(2, -3, 0, 2 * radius, threshold);
  // balls.addball(2, -5, 0, 2 * radius, threshold);
  // balls.addball(2, -7, 0, 2 * radius, threshold);

  var mesh = balls.metaballsmesh(0.5, 25, 1000);
  rotate(mesh, - 250, 5.4, 20);

  clear(img);

  //  set mesh = balls.metaballsmesh(20, 400);
  //  show(mesh);
  var show_video(directory) =
    {
      var files = list_files(directory);
      var ancre = loadimage("input/ancre2.gif");
      drawimage(img, ancre, 700, 0, 1, 1);
      var frame = 0;
      for(file in files)
	{
	  set frame = 1 + frame;
	  if(not(frame % speed)){
	    var image = loadimage(file);
	    var small = image.resize(-20, -20);
	    drawimage(img, small, 100, 200, 1, 0);
	    drawimage(img, small, 500, 200, 0.2, 0);
	    snapshot();
	  }
	  else;
	}
    }

  show_video("videoextracts/testvideoiphone");
}
