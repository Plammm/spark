//arret de bus
//dinosaur 3d

var a = -0.5;

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
      rectangle(img, 40 + 300, 20, 50 + 380, 80, color(100, 100, 100), 1);
      for(ti in seq(0, 6)){
	var opacity = 1 - (((3. - ti) / 4) * ((3. - ti) / 4));
	img.text(50 + 100 * ti, 30, string((n + ti) - 3), white, opacity, 40);
      }
    }
  else;
};

var black = color(0, 0, 0);
var blue = color(0, 0, 255);
var red = color(255, 0, 0);
var green = color(0, 255, 0);
var white = color(255, 255, 255);
var img = newimg(800, 600, 1, 3, 0);
var disp = newdisplay(img);


var stay(time) =
{
  for(toto in seq(0, rate * time - 1))
    snapshot();
};

var title(x, s) =
{
  var max = 5.0 * rate;
  for(ti in seq(0, max)){
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
    img.drawmesh(mesh, 200, 200, 0, alpha, beta, gamma, 700);
    snapshot();
  }
};

var raster() =
{
  var ploum(col) =
  {
    for(toto in seq(0, rate - 1)){
      var size = 100;
      for(i in seq(0, 7))
	for(j in seq(0, 5)){
	  rectangle(img, size * i, size * j, size * (i + 1) - 1, size * (j + 1) - 1, col(i, j), 0.02 * (i + 1) + 0.02 * (j + 1));
	}
      snapshot();
    };
  }
  var col1(i, j) = color(255 - i * 30, j * 50, i * 30);
  var col2(i, j) = color(i * 30, j * 50, i * 30);
  var col3(i, j) = color(j*50, j * 50, i * 30);
  var col4(i, j) = color(255 - j*50, j * 50, i * 30);
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
  if (storedimage)
    drawimage(img, storedimage, 0, 0, 1);
  else
    {}
};
