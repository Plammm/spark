// rue simple
// Fif marche
// apparition d'un abribus
// affiche

var mysterymachine = loadimage("/home/fmaurel/prog/spark/input/mysterymachine.png");

var grey(i) = color(i, i, i);

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

{
  var road = color(200, 200, 200);
  var rue = new3d();
  streetit(rue);

  store();

  var abribus = new3d();
  var brown = color(150, 100, 40);
  plane(abribus, 300, 195, 10, 10, 1500, -200, 700, brown, 0.9);
  plane(abribus, 100, 150, 10, 10, 1520, -180, 680, grey(200), 0.9);
  plane(abribus, 100, 150, 10, 10, 1640, -180, 680, grey(200), 0.9);
  hplane(abribus, 300, 195, 10, 10, 1500, -200, 500, brown, 0.9);

  var deltax = -10;
  var deltay = 2.6;
  for (i in seq (0, 100)){
    restore();
    img.drawmesh(rue, i * deltax, 600 + i * deltay, 0, -0.5, 0.5, 0, 40000);
    img.drawmesh(abribus, i * deltax, 600 + i * deltay, 0, -0.5, 0.5, 0, 40000);
    img.drawimage(mysterymachine, -deltax * (i /2), 300 - (deltay * i) / 2, 1);

    stay(0.1);
  }

  store();

  stay(2);

  restore();
  img.text(50, 100, "Mais que vois-je ?", black, 1, 100);
  stay(5);

  restore();
  img.text(50, 100, "Un abribus !", black, 1, 100);
  stay(3);


  //restore();
  //img.drawmesh(rue, -100, 600, 0, -0.5, 0.5, 0, 10);
  //stay(1);

  //img.drawmesh(rue, 200, 200, 0, -0.5, 5, 0);

  //rotate(rue);
  //  img.drawmesh(rue, 200, 200, 100, 3, 2, 1);
  snapshot();
  stay(5);
}
