var fill_matthieu(walk) =
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
  var pt_leftknee = pt_lefthip.link(-0.5, 4, 0);
  var pt_lefttoe = pt_leftknee.link(-0.5, 4, 0);

  var pt_righthip = pt_thigh.link(1, 1, 0);
  var pt_rightknee = pt_righthip.link(0.5, 4, 0);
  var pt_righttoe = pt_rightknee.link(0.5, 4, 0);

  balls.add(1, 10.5, grey(200), pt_head, pt_neck, pt_plexus);
  balls.add(1, 10.5, black, pt_headbot);
  balls.add(1, 10.5, brown, pt_neck, pt_plexus, pt_leftshoulder, pt_leftelbow, pt_leftarm);
  balls.add(1, 10.5, brown, pt_neck, pt_plexus, pt_rightshoulder, pt_rightelbow, pt_rightarm);
  balls.add(1.5, 10.5, red, pt_plexus, pt_thigh, pt_leftshoulder, pt_rightshoulder);
  balls.add(1.5, 10.5, blue, pt_plexus, pt_thigh, pt_lefthip, pt_leftknee);
  balls.add(1.5, 10.5, blue, pt_lefthip, pt_leftknee, pt_lefttoe);
  balls.add(1.5, 10.5, blue, pt_plexus, pt_thigh, pt_righthip, pt_rightknee);
  balls.add(1.5, 10.5, blue, pt_righthip, pt_rightknee, pt_righttoe);

  var add(vector, depth) =
  {
    pt_leftknee.translate(0, 0, -depth);
    pt_rightknee.translate(0, 0, depth);
    vector.push(balls.mesh(0.5, 25, 100));
  }

  var step = 0.25;
  var nbsteps = 4;

  add(walk, 0);
  for(i in seq(0, nbsteps -1))
    add(walk, step);
  for(i in seq(0, 2 * nbsteps -1))
    add(walk, -step);
  for(i in seq(0, nbsteps -2))
    add(walk, step);
}
