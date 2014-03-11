var play_dino() =
{

  var longshow(x, y, s, col, opacity, size) =
  {
    for(i in seq(1, s.length())){
      img.text(x, y, s.sub(0, i), col, opacity, size);
      stay(0.1);
    }
  }

  {
    var intro = newvector();
    intro.push("A l'aube de l'an 2000...");
    intro.push("Et au lieu de faire des trucs super balezes et mega importants");
    intro.push("(comme il fera ensuite)");
    intro.push("Matthieu travaille sur des objets en 3D.");
    intro.push("Ce travail s'est perdu...");
    intro.push("");
    intro.push("Il a donc fallu le refaire.");
    intro.push("");
    intro.push("(c'est Matthieu qui va etre content)");

    var size = 30;
    for(j in seq(0, intro.length() - 1)){
      longshow(100, 100 + size * j, intro.get(j), white, 1, size);
      stay(0.5);
    }
  }

  var dino = loadmesh("/home/fmaurel/prog/spark/input/dino2.off", color(0, 255, 0), 0.5);
  var queuedino = loadmesh("/home/fmaurel/prog/spark/input/dino2.off", color(0, 255, 0), 0.5);

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

    for(tt in seq(0, speed, 30)){
      dino.shrink_from(0, -35, 25, 0, 0.0, 0.01, tt / 100.);
      queuedino.shrink_from(0, -35, 25, 0, 0.0, 0.01, tt / 100.);
      reset();
      img.drawmesh(dino, 640, 400, - 4 * toto, alpha, beta, gamma, 700);
      snapshot();
    }
    stay(5);

    queuedino.select(0, -35, 25, 0, 0.0, 0.01);
    dino.select(0, -35, 25, 0, 0.0, -0.01);

    for(tt in seq(0, speed, 64)){
      reset();
      set beta = 5.4 + toto * 0.05 + tt / 40.;
      img.drawmesh(queuedino, 640 + 5 * tt, 400 - 3 * tt, - 4 * toto - 3 * tt, alpha, beta + tt / 20., gamma, 700);
      img.drawmesh(dino, 640, 400, - 4 * toto, alpha, beta, gamma, 700);
      snapshot();
    }

    stay(2);

    for(ttt in seq(0, speed, 50)){
      reset();
      var tt = 64;
      set beta = 5.4 + toto * 0.05 + tt / 40.;
      img.drawmesh(queuedino, 640 + 5 * tt - 5 * ttt, 400 - 3 * tt + 3 * ttt, - 4 * toto - 3 * tt, alpha, beta + tt / 20., gamma, 700);
      img.drawmesh(dino, 640, 400, - 4 * toto, alpha, beta, gamma, 700);
      snapshot();
    }
    stay(5);
  }
}
