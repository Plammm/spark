// rue simple
// Fif marche
// apparition d'un abribus
// affiche

var play_abribus() =
{
  var mysterymachine = loadimage("/home/fmaurel/prog/spark/input/mysterymachine2.gif");
  var rat1 = loadimage("/home/fmaurel/prog/spark/input/rat1.gif");
  var rat1small = rat1.resize(-50, -50);
  var rat1smallsmall = rat1.resize(-20, -20);

  var streetit(rue) =
  {
    for (i in seq (1, 9))
      hplane(rue, 4000, 60, 10, 10, 0, 0, 50 * (i + 9), color(150, 170, 150), 0.9);
    for (i in seq (0, 9))
      hplane(rue, 4000, 60, 10, 10, 0, 0, 50 * i, grey(255 - 8 * i), 0.9);
    for (i in seq (1, 9))
      hplane(rue, 4000, 60, 10, 10, 0, 0, -50 * i, color(150, 170, 150), 0.9);
    for (i in seq (0, 9))
      hplane(rue, 200, 40, 10, 10, 300 * i, -100, 100, white, 0.9);
  }

  var abribusit(abribus, x, y, z) =
  {
    var subd = 1;
    plane(abribus, 260, 25, subd, subd, x, y -200, z, brown, 0.9);
    plane(abribus, 20, 150, subd, subd, x, y -175, z, brown, 0.9);
    plane(abribus, 20, 150, subd, subd, x + 120, y -175, z, brown, 0.9);
    plane(abribus, 20, 150, subd, subd, x + 240, y -175, z, brown, 0.9);
    plane(abribus, 260, 25, subd, subd, x, y -25, z, brown, 0.9);

    plane(abribus, 100, 150, subd, subd, x + 20, y -175, z, grey(200), 0.9);
    plane(abribus, 100, 150, subd, subd, x + 140, y -175, z, grey(200), 0.9);
    hplane(abribus, 260, 195, subd, subd, x, y -200, z - 200, brown, 0.9);
  }

  {
    var road = color(200, 200, 200);
    var rue = new3d();
    streetit(rue);
    var abribus = new3d();
    abribusit(abribus, 0, 0, 700);

    store();
    var imgfif2 = loadimage("/home/fmaurel/prog/spark/input/fif2.jpg");

    var deltax = -10;
    var deltay = 2.6;

    var showabribus(time, lambda) =
    {
      var i = time / 2.5;
      //      img.drawmesh(abribusback, (1 - lambda) * i * deltax - lambda * 2300, (700 + i * deltay) * (1 - lambda) + lambda * 400, 0, -0.5 * (1 - lambda), 0.5 * (1 - lambda), 0, 40000);
      //      img.drawmesh(abribus, (1 - lambda) * i * deltax - lambda * 2300, (700 + i * deltay) * (1 - lambda) + lambda * 400, -lambda * 50000, -0.5 * (1 - lambda), 0.5 * (1 - lambda), 0, 40000);

      //      img.drawmesh(abribus, (1 - lambda) * i * deltax - lambda * 2300, (700 + i * deltay) * (1 - lambda) + lambda * 400, 0, -0.5 * (1 - lambda), 0.5 * (1 - lambda), 0, 40000);

      img.drawmesh(abribus, 2500 + (1 - lambda) * i * deltax - lambda * 2300, (80 + i * deltay) * (1 - lambda) + lambda * 700, -lambda * 25000, -0.5 * (1 - lambda), 0.5 * (1 - lambda), 0, 40000);
    }

    var showrue(time, withabribus) =
    {
      var i = time / 2.5;
      img.drawmesh(rue, i * deltax, 700 + i * deltay, 0, -0.5, 0.5, 0, 40000);
      if (withabribus)
	showabribus(time, 0);
      else;
      img.drawimage(mysterymachine, -deltax * (i * 3), 430 - (deltay * i) * 3, 1, 1);

      set i = time / 2.0;
      img.drawimage(rat1small, 400 + deltax * (i - 100), 550 + deltay * (i - 100), 1, 1);
      img.drawimage(rat1small, 400 + deltax * (i - 150), 630 + deltay * (i - 150), 1, 1);
      img.drawimage(rat1small, 400 + deltax * (i - 180), 600 + deltay * (i - 180), 1, 1);
      img.drawimage(rat1small, 400 + deltax * (i - 240), 650 + deltay * (i - 240), 1, 1);

      img.drawimage(rat1small, 400 + deltax * (i - 10), 230 + deltay * (i - 10), 1, 1);

      img.drawimage(rat1smallsmall, 400 + deltax * (i - 150), 180 + deltay * (i - 150), 1, 1);
      img.drawimage(rat1smallsmall, 400 + deltax * (i - 150), 180 + deltay * (i - 150), 1, 1);

      img.drawimage(rat1smallsmall, 400 + deltax * (i - 250), 200 + deltay * (i - 250), 1, 1);
      img.drawimage(rat1smallsmall, 400 + deltax * (i - 200), 150 + deltay * (i - 200), 1, 1);

      img.drawimage(rat1smallsmall, 400 + deltax * (i - 150), 180 + deltay * (i - 150), 1, 1);
      img.drawimage(rat1smallsmall, 400 + deltax * (i - 70), 130 + deltay * (i - 60), 1, 1);
      img.drawimage(rat1smallsmall, 400 + deltax * (i - 40), 170 + deltay * (i - 35), 1, 1);
      img.drawimage(rat1smallsmall, 400 + deltax * (i - 20), 180 + deltay * (i - 15), 1, 1);
      img.drawimage(rat1smallsmall, 400 + deltax * (i + 20), 150 + deltay * (i + 25), 1, 1);
      img.drawimage(rat1smallsmall, 400 + deltax * (i + 50), 160 + deltay * (i + 55), 1, 1);
    }

    var maxtime = 400;
    for (time in seq (0, speed, maxtime)){
      restore();
      showrue(time, 1);
      img.drawimage(imgfif2, - deltax * (time / 5.0), 550 - (deltay * (time / 5.0)), 1, 0);
      snapshot();
      //stay(0.1);
    }

    //    clear(img);
    //    showrue(maxtime, 0);
    store();
    for (time in seq (0, speed, 50)){
      img.clear();
      img.drawimage(storedimage, 0, 0, 1 - time / 50., 0);
      showabribus(maxtime, 0);
      snapshot();
    }

    store();
    for (time in seq (0, speed, 50)){
      img.clear();
      //      drawimage(img, storedimage, 0, 0, 1 - time / 50., 0);
      showabribus(maxtime, time / 50.);
      snapshot();
    }

    stay(2);

    // restore();
    // img.text(50, 100, "Mais que vois-je ?", black, 1, 100);
    // stay(5);

    // restore();
    // img.text(50, 100, "Un abribus !", black, 1, 100);
    // stay(3);


    //restore();
    //img.drawmesh(rue, -100, 600, 0, -0.5, 0.5, 0, 10);
    //stay(1);

    //img.drawmesh(rue, 200, 200, 0, -0.5, 5, 0);

    //rotate(rue);
    //  img.drawmesh(rue, 200, 200, 100, 3, 2, 1);
    snapshot();
    stay(5);
  }
}
