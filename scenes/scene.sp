var imgens = loadimage("/home/fmaurel/prog/spark/input/ens1.jpg");
var imgfif1 = loadimage("/home/fmaurel/prog/spark/input/fif1.jpg");
var imgfif2 = loadimage("/home/fmaurel/prog/spark/input/fif2.jpg");

abribus();

raster();

store();
for (i in seq(0, 30)){
  restore();
  drawimage(img, imgens, 100, 100, i / 30.0, 0);
  snapshot();
 };

stay(5);
restore();
for (i in seq(0, 100)){
  restore();
  drawimage(img, imgfif1, 100 + i, 100, 1, 0);
  snapshot();
 };
//stay(5);
restore();
drawimage(img, imgfif2, 100, 100, 1, 0);

snapshot();

stay(5);
title(300, "Le Dino");
set year = 1997;
play_dino();
raster();
raster();

