var dino = loadmesh("/home/fmaurel/prog/spark/input/dino2.off", color(0, 255, 0), 0.5);
var imgens = loadimage("/home/fmaurel/prog/spark/input/ens1.jpg");
var imgfif1 = loadimage("/home/fmaurel/prog/spark/input/fif1.jpg");
var imgfif2 = loadimage("/home/fmaurel/prog/spark/input/fif2.jpg");

raster();

store();
for (i in seq(0, 30)){
  restore();
  drawimage(img, imgens, 100, 100, i / 30.0);
  snapshot();
 };

stay(5);
restore();
for (i in seq(0, 100)){
  restore();
  drawimage(img, imgfif1, 100 + i, 100, 1);
  snapshot();
 };
//stay(5);
restore();
drawimage(img, imgfif2, 100, 100, 1);

snapshot();

stay(5);
title(300, "Le Dino");
set year = 1997;
rotate(dino);
raster();
raster();

