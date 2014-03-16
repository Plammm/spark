//arret de bus
//dinosaur 3d

var speed = 1;

var rate = 25;
var imageid = 0;
var waitlength = 1000.0 / rate;
var snapshot() =
{
  img.display(disp);
  disp.wait(waitlength);
  img.savepng(imageid);
  set imageid = imageid + 1;
};

var year = 0;

var reset() =
{
  img.clear();
  frise(year);
}

var frise(n) =
{
  var center = 300;
  if (n)
    {
      for(ti in seq(0, 11)){
	var opacity = 1 - (((10. - ti) / 12) * ((10. - ti) / 12));
	if (ti - 10)
	  img.rectangle(40 + ti * 100, 20, 30 + (ti + 1) * 100, 80, color(20, 20, 20), 1);
	else
	  img.rectangle(40 + ti * 100, 20, 30 + (ti + 1) * 100, 80, color(100, 100, 100), 1);
	img.text(50 + 100 * ti, 30, string((n + ti) - 10), white, opacity, 40);
      }
    }
  else;
};


var grey(i) = color(i, i, i);
var black = color(0, 0, 0);
var blue = color(0, 0, 255);
var red = color(255, 0, 0);
var green = color(0, 255, 0);
var white = color(255, 255, 255);
var brown = color(150, 100, 40);
var darkbrown = color(75, 50, 20);
var img = newimg(1280, 720, 1, 3, 0);
var disp = img.newdisplay();

var stay(time) =
{
  for(toto in seq(0, speed, rate * time - 1))
    snapshot();
};

var title(x, s) =
{
  var max = 5.0 * rate;
  for(ti in seq(0, speed, max)){
    img.clear();
    var toto = 3 * ((33 * ti) / max);
    img.text(x - toto, 300 - toto, s, white, toto / max, toto);
    snapshot();
  }
};

var rotate(mesh, z0, beta0, beta1) =
{
  var alpha = -0.5;
  var gamma = 0.0;
  for(toto in seq(0, speed, 100)){
    var beta = beta0 + beta1 * (toto / 100.0);
    reset();
    img.drawmesh(mesh, 640, 400, z0 - 4 * toto, alpha, beta, gamma, 700);
    snapshot();
  }
};

var raster() =
{
  var ploum(col) =
  {
    for(toto in seq(0, speed, rate - 1)){
      var size = 100;
      for(i in seq(0, 12))
	for(j in seq(0, 7)){
	  img.rectangle(size * i, size * j, size * (i + 1) - 1, size * (j + 1) - 1, col(i, j), speed * 0.02 * (i + 1) + 0.02 * (j + 1));
	}
      snapshot();
    };
  }
  var col1(i, j) = color(255 - i * 20, j * 30, i * 20);
  var col2(i, j) = color(i * 20, j * 30, i * 20);
  var col3(i, j) = color(j * 30, j * 30, i * 20);
  var col4(i, j) = color(255 - j * 30, j * 30, i * 20);
  ploum(col1);
  ploum(col2);
  ploum(col3);
  ploum(col4);
};


var storedimage = 0;
var store() = {
  set storedimage = img.copy();
};

var restore() = {
  //if (storedimage)
    img.drawimage(storedimage, 0, 0, 1, 0);
    // else
    {}
};
