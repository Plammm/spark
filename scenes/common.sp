//arret de bus
//dinosaur 3d

var speed = 1;

var rate = 25;
var imageid = 0;
var waitlength = 1000.0 / rate;
var snapshot() =
{
  display(img, disp);
  wait(disp, waitlength);
  img.savebmp(imageid);
  set imageid = imageid + 1;
};

var year = 0;

var reset() =
{
  clear(img);
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
	  rectangle(img, 40 + ti * 100, 20, 30 + (ti + 1) * 100, 80, color(20, 20, 20), 1);
	else
	  rectangle(img, 40 + ti * 100, 20, 30 + (ti + 1) * 100, 80, color(100, 100, 100), 1);
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
var img = newimg(1280, 720, 1, 3, 0);
var disp = newdisplay(img);


var stay(time) =
{
  for(toto in seq(0, speed, rate * time - 1))
    snapshot();
};

var title(x, s) =
{
  var max = 5.0 * rate;
  for(ti in seq(0, speed, max)){
    clear(img);
    var toto = 3 * ((33 * ti) / max);
    img.text(x - toto, 300 - toto, s, white, toto / max, toto);
    snapshot();
  }  
};

var rotate(mesh) =
{
  var alpha = -0.5;
  var beta = 5.4;
  var gamma = 0.0;
  for(toto in seq(0, 99)){
    //set alpha = alpha - 0.05;
    set beta = beta + 0.05;
    //set gamma = gamma + 0.03;
    reset();
    //img.text(100, 100, (string(alpha) + " ") +string(beta), white, 1, 30);
    img.drawmesh(mesh, 400, 300, 0, alpha, beta, gamma, 700);
    snapshot();
  }
};

var raster() =
{
  var ploum(col) =
  {
    for(toto in seq(0, rate - 1)){
      var size = 100;
      for(i in seq(0, 12))
	for(j in seq(0, 7)){
	  rectangle(img, size * i, size * j, size * (i + 1) - 1, size * (j + 1) - 1, col(i, j), 0.02 * (i + 1) + 0.02 * (j + 1));
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
  set storedimage = copyimage(img);
};

var restore() = {
  //if (storedimage)
    drawimage(img, storedimage, 0, 0, 1, 0);
    // else
    {}
};
