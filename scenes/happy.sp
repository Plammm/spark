{
  var balls = newmetaball();

  var radius = 1.4;
  var threshold = 1.5;

  var point(x, y, z, radius, threshold) = balls.addball(x, y, z, radius, threshold);
  var line(x1, y1, z1, x2, y2, z2, n, radius, threshold) =
    {
      for(i in seq(0, n)){
	var lambda = i / (0.0 + n);
	point((1 - lambda) * x1 + lambda * x2,
	      (1 - lambda) * y1 + lambda * y2,
	      (1 - lambda) * z1 + lambda * z2, radius, threshold);
      }
    }

  balls.addball(0, -12, 0, 4, 2);

  //line(0,-8,0,4,-4,0,3, radius, threshold);
  line(0,-8,0, -4,5,0, 15, 1, 1.1);
  line(0,-8,0, 4,5,0, 15, 1, 1.1);
  line(0,0,0,0,-10,0,10, radius, threshold);
  line(0,0,0,-3,20,0,15, radius, threshold);
  line(0,0,0,3,20,0,15, radius, threshold);

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
  show(mesh);

  //  set mesh = balls.metaballsmesh(20, 400);
  //  show(mesh);
  var show_video(directory) =
    {
      var files = list_files(directory);
      for(file in files)
	{
	  var image = loadimage(file);
	  var small = image.resize(-20, -20);
	  drawimage(img, small, 100, 200, 1, 0);
	  drawimage(img, small, 500, 200, 0.2, 0);
	  snapshot();
	}
    }

  //  show_video("videoextracts/testvideoiphone");
}
