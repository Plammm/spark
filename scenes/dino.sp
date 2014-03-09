var play_dino() =
{
  var dino = loadmesh("/home/fmaurel/prog/spark/input/dino2.off", color(0, 255, 0), 0.5);
  //  dino.color_from(0, 0, 150, 5, 0, -2, 0, 0, 255);
  //dino.shrink_from(0, -40, 30, 0, 0.0, 0.1, 0.5);
  //  dino.color_from(0, 0, 30, 5, 0, -6, 0, 0, 255);
  {
    var alpha = -0.5;
    var beta = 5.4;
    var gamma = 0.0;
    var toto = 0;
    for(tt in seq(0, speed, 99)){
      set beta = 5.4 + toto * 0.05;
      set toto = tt;
      reset();
      img.drawmesh(dino, 640, 400, - 4 * toto, alpha, beta, gamma, 700);
      snapshot();
    }

    for(tt in seq(0, 30)){
      dino.shrink_from(0, -35, 25, 0, 0.0, 0.01, tt / 100.);
      reset();
      img.drawmesh(dino, 640, 400, - 4 * toto, alpha, beta, gamma, 700);
      snapshot();
    }
    stay(5);
  }
}
