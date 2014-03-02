var play_dino() =
{
  var dino = loadmesh("/home/fmaurel/prog/spark/input/dino2.off", color(0, 255, 0), 0.5);

  {
    var alpha = -0.5;
    var beta = 5.4;
    var gamma = 0.0;
    for(toto in seq(0, speed, 99)){
      set beta = 5.4 + toto * 0.05;
      reset();
      img.drawmesh(dino, 640, 400, - 4 * toto, alpha, beta, gamma, 700);
      snapshot();
    }
  }
}
