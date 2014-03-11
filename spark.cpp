#include <iostream>
#include <cctype>
#include <vector>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "CImg.h"
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <typeinfo>
#include <memory>
#include <boost/filesystem.hpp>

#include <spunk.hpp>


#ifndef cimg_imagepath
#define cimg_imagepath "../CImg/CImg-1.5.7/examples/img/"
#endif
#define faileval {cout << "Line: " << __LINE__ << endl; throw new EvalException;}
#define failparse {cout << "Line: " << __LINE__ << " " << nextPos << endl; throw new ParseException;}
#define ccout(s) {cout << "Line: " << __LINE__ << " NextPos: " << nextPos << " " << s << endl; }
#define here //{cout << "Here: " << __LINE__ << endl;}

#define open_parameters unsigned int n = 0; unsigned int opened_parameters = 0;
#define close_parameters if (parameters.size() != opened_parameters + n) faileval;
#define getany(any,v) if (parameters.size() < n) faileval; ValueAny<any>* v ## ptr = dynamic_cast<ValueAny<any>*>(parameters[n]); if (v ## ptr == 0) faileval; any v = v ## ptr->value; n++;
#define getptr(any,v) getany(any*,v)
#define getdouble(v) if (parameters.size() < n) faileval; ValueAny<double>* v ## ddd = doubleValue(parameters[n]); if (v ## ddd == 0) faileval; double v = v ## ddd->value; n++;
#define getint(v) if (parameters.size() < n) faileval; ValueAny<int>* v ## ddd = intValue(parameters[n]); if (v ## ddd == 0) faileval; int v = v ## ddd->value; n++;

//#define getint(v) ValueAny<int>* v ## ptr = dynamic_cast<ValueAny<int>*>(parameters[n]); if (v ## ptr == 0) faileval; int v = v ## ptr->value; n++;

//if(!parameters[n]->isNum) faileval; int v = parameters[n]->intValue;n++;
//#define getdouble(v) if(!parameters[n]->isNum) faileval; double v = parameters[n]->doubleValue;n++;
//#define get(string,v) if(parameters[n]->kind != String) faileval; string v = parameters[n]->stringValue;n++;

using namespace std;
using namespace cimg_library;

namespace Image
{
  struct Plane3d {
    typedef typename cimg::last<float,float>::type floatT;

    static CImg<float> plane3d(CImgList<float>& primitives,
                                const float size_x=100, const float size_z=100,
                                const unsigned int subdivisions_x=10, const unsigned int subdivisions_y=10) {
      primitives.assign();
      if (!subdivisions_x || !subdivisions_y) return CImg<floatT>();
      CImgList<floatT> vertices;
      const unsigned int w = subdivisions_x + 1, h = subdivisions_y + 1;
      const float fx = (float)size_x/subdivisions_x, fy = (float)size_z/subdivisions_y;
      for (unsigned int y = 0; y<h; ++y)
        for (unsigned int x = 0; x<w; ++x)
          CImg<floatT>::vector(fx*x,fy*y,0).move_to(vertices);
      for (unsigned int y = 0; y<subdivisions_y; ++y) for (unsigned int x = 0; x<subdivisions_x; ++x) {
          const int off1 = x+y*w, off2 = x+1+y*w, off3 = x+1+(y+1)*w, off4 = x+(y+1)*w;
          CImg<float>::vector(off1,off2, off3, off4).move_to(primitives);
          CImg<float>::vector(off1,off4, off3, off2).move_to(primitives);
        }
      return vertices>'x';
    }

    static CImg<float> hplane3d(CImgList<unsigned int>& primitives,
                                const float size_x=100, const float size_z=100,
                                const unsigned int subdivisions_x=10, const unsigned int subdivisions_y=10) {
      primitives.assign();
      if (!subdivisions_x || !subdivisions_y) return CImg<floatT>();
      CImgList<floatT> vertices;
      const unsigned int w = subdivisions_x + 1, h = subdivisions_y + 1;
      const float fx = (float)size_x/subdivisions_x, fy = (float)size_z/subdivisions_y;
      for (unsigned int y = 0; y<h; ++y)
        for (unsigned int x = 0; x<w; ++x)
          CImg<floatT>::vector(fx*x,0, fy*y).move_to(vertices);
      for (unsigned int y = 0; y<subdivisions_y; ++y) for (unsigned int x = 0; x<subdivisions_x; ++x) {
          const int off1 = x+y*w, off2 = x+1+y*w, off3 = x+1+(y+1)*w, off4 = x+(y+1)*w;
          CImg<unsigned int>::vector(off1,off2, off3, off4).move_to(primitives);
          CImg<unsigned int>::vector(off1,off4, off3, off2).move_to(primitives);
        }
      return vertices>'x';
    }

    static void Draw(int argc, char **argv,
                     const float size_x=1, const float size_y=1,
                     const unsigned int subdivisions_x=10, const unsigned int subdivisions_y=10) {
      const char* expression = "x + y * y";
      const char *const formula = cimg_option("-f",expression,"Formula to plot");
      CImgList<unsigned int> faces3d;
      faces3d.assign();
      if (!subdivisions_x || !subdivisions_y) return;
      CImgList<unsigned char> colors;
      CImgList<floatT> vertices;
      CImgList<floatT> opacities;
      const unsigned int w = subdivisions_x + 1, h = subdivisions_y + 1;
      const float fx = (float)size_x/w, fy = (float)size_y/h;
      // double f0 = cimg::eval("x + y * y", 0, 0, 0, 0);
      // if (10 < f0)
      //  throw f0;
      for (unsigned int y = 0; y<h; ++y)
        for (unsigned int x = 0; x<w; ++x){
          double z = cimg::eval(formula, fx*x, fy*y, 0, 0);
          //double z = cimg::eval(0, fx*x, fy*y, 0, 0);
          //z = x + y * y;
          CImg<floatT>::vector(fx*x,fy*y,z).move_to(vertices);
        }
      for (unsigned int y = 0; y<subdivisions_y; ++y)
        for (unsigned int x = 0; x<subdivisions_x; ++x) {
          const int off1 = x+y*w, off2 = x+1+y*w, off3 = x+1+(y+1)*w, off4 = x+(y+1)*w;
          CImg<unsigned int>::vector(off1,off4,off3,off2).move_to(faces3d);
          colors.insert(CImg<unsigned char>::vector(0,0,255));
          opacities.insert(CImg<floatT>::vector(1.0f));
        }
      if (true){
        for (unsigned int y = 0; y<h; ++y)
          for (unsigned int x = 0; x<w; ++x){
            double z = 0;
            CImg<floatT>::vector(fx*x,fy*y,z).move_to(vertices);
          }
        for (unsigned int y = 0; y<subdivisions_y; ++y)
          for (unsigned int x = 0; x<subdivisions_x; ++x) {
            const int off1 = x+y*w, off2 = x+1+y*w, off3 = x+1+(y+1)*w, off4 = x+(y+1)*w;
            int off = h * w;
            CImg<unsigned int>::vector(off + off1,off + off4,off + off3,off + off2).move_to(faces3d);

            colors.insert(CImg<unsigned char>::vector(240, 240, 240));
            opacities.insert(CImg<floatT>::vector(0.4f));
            //CImg<unsigned int>::vector(off + off1,off + off4,off1,off4).move_to(faces3d);
          }
      }

      const CImg<float> points3d = vertices>'x';
      //CImg<unsigned char> graph = CImg<unsigned char>().display_object3d("Plane3d", points3d, faces3d, colors, opacities, true);
      const CImg<unsigned char> visu = CImg<unsigned char>(3,512,512,1).fill(230,230,255).permute_axes("yzcx");
      visu.display_object3d("Plane3d", points3d, faces3d, colors, opacities, true);
      visu.save_bmp("/home/fmaurel/prog/spark/test1.bmp");

    }
  } plane3d;

  class MyMesh{
  public:
    CImg<float> vertices;
    CImgList<float> primitives;
    CImgList<float> colors;
    CImg<float> opacities;
    MyMesh(){}
    MyMesh* vertex(float x, float y, float z){
      //vertices.append(CImg<float>::vector(x,y,z));
      CImg<float>::vector(x,y,z).move_to(vertices);
      return this;
    }
    MyMesh* primitive(int v1, int v2, int v3){
      CImg<int>::vector(v1,v2,v3).move_to(primitives);
      return this;
    }
    MyMesh* readOFF(string filename, CImg<unsigned char>& col, double opacity){
      std::ifstream infile(filename.data());
      string header;
      infile >> header;
      int lv, lp, n;
      infile >> lv >> lp >> n;
      cout << header << endl;
      cout << lv << " " << lp << " " << n << endl;
      CImgList<float> verticeslist;
      for (int i = 0; i < lv; i++){
        double x, y, z;
        infile >> x >> y >> z;
        CImg<float>::vector(x,-y,-z).move_to(verticeslist);
      }
      for (int i = 0; i < lp; i++){
        int n, x, y, z;
        infile >> n >> x >> y >> z;
        primitive(x, y, z);
      }
      infile.close();
      vertices = (verticeslist>'x').shift_object3d();
      cout << "MESH read. " << verticeslist.size() << " " << vertices.size() << " " << primitives.size() << endl;
      colors = CImgList<unsigned char>(primitives.size(), col);
      opacities = CImg<float>(primitives.size(), opacity);
      return this;
    }
    void colorfrom(float xc, float yc, float zc, float xf, float yf, float zf, float r0, float g0, float b0){
      colors.assign();
      for(unsigned int i = 0; i < primitives.size(); i++){
        CImg<float> f = primitives[i];
	float x = vertices(f(0), 0);
	float y = vertices(f(0), 1);
        float z = vertices(f(0), 2);
	int r = r0 + xf * abs(x - xc);
	int g = g0 + yf * abs(y - yc);
	int b = b0 + zf * abs(z - zc);
	if (r < 0) r = 0;
	if (g < 0) g = 0;
	if (b < 0) b = 0;
	colors.insert(CImg<unsigned char>::vector(r, g, b));
      }
    }
    void shrinkfrom(float xc, float yc, float zc, float xf, float yf, float zf, float coef){
      //colors.assign();
      unsigned int size = vertices.size() / 3;
      for(unsigned int i = 0; i < size; i++){
        float x = vertices(i, 0);
        float y = vertices(i, 1);
        float z = vertices(i, 2);
	float dist = xf * (x - xc) * (x - xc) + yf * (y - yc) * (y - yc) + zf * (z - zc) * (z - zc);
	float lambda = coef * (1 - dist);
	if (lambda < 0)
	  lambda = 0;
	vertices[i] = (1 - lambda) * x + lambda * xc;
	vertices[size + i] = (1 - lambda) * y + lambda * yc;
	vertices[2 * size + i] = (1 - lambda) * z + lambda * zc;
      }
    }
    void select(float xc, float yc, float zc, float xf, float yf, float zf){
      CImgList<float> newprimitives;
      CImgList<float> newcolors;
      CImg<float> newopacities;
      for(unsigned int i = 0; i < primitives.size(); i++){
        CImg<float> f = primitives[i];
	float x = vertices(f(0), 0);
	float y = vertices(f(0), 1);
        float z = vertices(f(0), 2);
	float dir = xf * (x - xc) + yf * (y - yc) + zf * (z - zc);
	if (dir < 0) continue;
	newcolors.insert(colors[i]);
	primitives[i].move_to(newprimitives);
      }
      colors = newcolors;
      primitives = newprimitives;
      //      newopacities = CImg<float>(newprimitives.size(), opacities(0));
      //opacities = newopacities;
    }
    void drawrotate(CImg<unsigned char>* img, int x, int y, int z, float alpha, float beta, float gamma, float focale){
      //      cout << "draw1 " << vertices.size() << endl;
      //      const CImg<float> rpoints = CImg<>::rotation_matrix(1,1,0,(alpha))*CImg<>::rotation_matrix(1,0,1,(beta))*CImg<>::rotation_matrix(0,1,1,(gamma))*((vertices>'x').shift_object3d());
      const CImg<float> rpoints = CImg<>::rotation_matrix(1,0,0,(alpha))*CImg<>::rotation_matrix(0,1,0,(beta))*CImg<>::rotation_matrix(0,0,1,(gamma))*((vertices));
      //      cout << "BB" << endl;
      //cout << "draw2 " << rpoints.size() << " " << endl;
      img->draw_object3d(x, y, z, rpoints, primitives, colors, opacities, 4, false, focale);
      //cout << "draw3" << endl;
      //      cout << "CC" << endl;
    }

    void show(){
      const CImg<unsigned char> visu = CImg<unsigned char>(3,512,512,1).fill(230,230,255).permute_axes("yzcx");
      CImg<float> view_matrix = CImg<>::identity_matrix(4);
      visu.display_object3d("SP 1",vertices,primitives,colors,opacities,true,4,4,false,
			    500.0f,0,0,-5000,0.5f,0.1f,true,view_matrix.data());
    }
  };

  unique_ptr<MyMesh> fmimageplane(const float size_x, const float size_y, const int x, const int y, const  int z, CImg<unsigned char> &texture, float opacity){
    CImgList<float> prims;
    prims.assign(1,1,4,1,1, 3,0,1,2);
    CImgList<float> vertices;
    CImg<float>::vector(0,0,0).move_to(vertices);
    CImg<float>::vector(0,size_y,0).move_to(vertices);
    CImg<float>::vector(size_x,size_y,0).move_to(vertices);
    CImg<float>::vector(size_x,0,0).move_to(vertices);
    CImg<float> pts = (vertices>'x').shift_object3d(x,y,z);
    CImgList<unsigned char> cols;
    cimglist_for(prims,p) {
      cols.insert(texture,~0U,true);
      prims[p].append(CImg<unsigned int>::vector(0,0,size_x-1,0,size_x-1,size_y-1,0,size_y-1),'y');
    }
    CImg<float> opacities = CImg<float>(prims.size(), opacity);
    unique_ptr<MyMesh> result(new MyMesh);
    result->primitives = prims;
    result->vertices = pts;
    result->colors = cols;
    result->opacities = opacities;
    return result;
  }

  unique_ptr<MyMesh> fmplane3d(const float size_x, const float size_y, const unsigned int subdivisions_x, const unsigned int subdivisions_y, const int x, const int y, const  int z, CImg<unsigned char> &color, float opacity){
    CImgList<float> prims;
    CImg<float> pts = Plane3d::plane3d(prims,size_x,size_y,subdivisions_x,subdivisions_y).shift_object3d(x,y,z);
    CImgList<unsigned char> cols = CImgList<unsigned char>(prims.size(), color);
    CImg<float> opacities = CImg<float>(prims.size(), opacity);
    unique_ptr<MyMesh> result(new MyMesh);
    result->primitives = prims;
    result->vertices = pts;
    result->colors = cols;
    result->opacities = opacities;
    return result;
  }

  unique_ptr<MyMesh> fmhplane3d(const float size_x, const float size_z, const unsigned int subdivisions_x, const unsigned int subdivisions_y, const int x, const int y, const int z, CImg<unsigned char> &color, float opacity){
    CImgList<unsigned int> prims;
    CImg<float> pts = Plane3d::hplane3d(prims,size_x,size_z,subdivisions_x,subdivisions_y).shift_object3d(x,y,z);
    CImgList<unsigned char> cols = CImgList<unsigned char>(prims.size(), color);
    CImg<float> opacities = CImg<float>(prims.size(), opacity);
    unique_ptr<MyMesh> result(new MyMesh);
    result->primitives = prims;
    result->vertices = pts;
    result->colors = cols;
    result->opacities = opacities;
    return result;
  }

  void append(MyMesh* scene, unique_ptr<MyMesh>& other){
    scene->vertices = scene->vertices.append_object3d(scene->primitives, other->vertices, other->primitives);
    scene->colors=(+scene->colors, other->colors);
    scene->opacities=(scene->opacities, other->opacities)>'x';
  }

  class supportPoint {
  public:
    float x, y, z;
    vector<supportPoint*> links;
  public:
    supportPoint(float x, float y, float z){
      this->x = x;
      this->y = y;
      this->z = z;
    }
    supportPoint* link(float dx, float dy, float dz){
      supportPoint* result = new supportPoint(x + dx, y + dy, z + dz);
      links.push_back(result);
      return result;
    }
    void translate(float dx, float dy, float dz){
      x += dx;
      y += dy;
      z += dz;
      for(unsigned int i = 0; i < links.size(); i++)
        links[i]->translate(dx, dy, dz);
    }
  };

  struct Ball {
    supportPoint* point;
    float radius;
    float threshold;
    Ball(supportPoint* point, float radius, float threshold){
      this->point = point;
      this->radius = radius;
      this->threshold = threshold;
    }
  };

  class MetaBall {
  public:
    vector<Ball*> balls;
    CImg<unsigned char>* color;
    void add(supportPoint* point, float radius, float threshold){
      balls.push_back(new Ball(point, radius, threshold));
    }
    float potential(const float x, const float y, const float z) {
      float potential = 0;
      for(unsigned int i = 0; i < balls.size(); i++){
        float dx = x - balls[i]->point->x;
        float dy = y - balls[i]->point->y;
        float dz = z - balls[i]->point->z;
        float r = (dx * dx + dy * dy + dz * dz) / balls[i]->radius;
        float v = 1 / (r);
        //      if (r < balls[i]->threshold && ( potential == 0 || potential < v))
        if (r < balls[i]->threshold)
          potential += v;//(1 - r * r) * (1 - r * r);
          //      potential += 1 - r * (r * (4 * r + 17) - 22) / 9;
      }
      return potential;
    }
  };

  class MetaMetaBall {
  private:
    vector<MetaBall*> balls;
  public:
    void add(MetaBall* ball){
      balls.push_back(ball);
    }
    inline float operator()(const float x, const float y, const float z) const {
      float potential = 0;
      for (unsigned int i = 0; i < balls.size(); i++){
        float pot = balls[i]->potential(x, y, z);
        if (potential < pot)
          potential = pot;
      }
      return potential;
    }
    MyMesh* mesh(float target, float r, int sub){
      MyMesh* result = new MyMesh();
      result->vertices = CImg<>::isosurface3d(result->primitives,*this,target,-r,-r,-r,r,r,r,sub);
      result->primitives.reverse_object3d();
      for(unsigned int i = 0; i < result->primitives.size(); i++){
        float weight = 0.00001;
        float r = 0;
        float g = 0;
        float b = 0;
        CImg<float> f = result->primitives[i];
        float x = result->vertices(f(0), 0);
        float y = result->vertices(f(0), 1);
        float z = result->vertices(f(0), 2);
        for(unsigned bid = 0; bid < balls.size(); bid++){
          float potential = balls[bid]->potential(x, y, z);
          weight += potential;
          r += potential * (*(balls[bid]->color))(0);
          g += potential * (*(balls[bid]->color))(1);
          b += potential * (*(balls[bid]->color))(2);
        }
        //cout << f(0) << " " << r << " " << g << " " << b << " " << weight << " " << endl;
        result->colors.insert(CImg<unsigned char>::vector(r / weight, g / weight, b / weight));
      }
      return result;
    }
  };
}

namespace Spark {
  using namespace Spunk;

  ValueAny<Image::supportPoint*>* newpoint(Image::supportPoint* point);
  ValueAny<CImg<unsigned char>*>* newimage(CImg<unsigned char>* point);
  ValueAny<Image::MyMesh*>* newmesh(Image::MyMesh* point);

  class FColor: public Spunk::FValue {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getint(p1);
      getint(p2);
      getint(p3);
      close_parameters;
      CImg<unsigned char>* color = new CImg<unsigned char>(CImg<unsigned char>::vector(p1, p2, p3));
      //cout << "COL create " << color->size() << endl;
      return newimage(color);
    }
    FColor(){
      name = "color";
    }
  } fcolor;

  class FPlane: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getany(Image::MyMesh*,scene);
      getint(p1);
      getint(p2);
      getint(p3);
      getint(p4);
      getint(p5);
      getint(p6);
      getint(p7);
      getptr(CImg<unsigned char>,p8);
      getdouble(p9);
      close_parameters;
      unique_ptr<Image::MyMesh> plane = Image::fmplane3d(p1, p2, p3, p4, p5, p6, p7, *p8, p9);
      Image::append(scene, plane);
      return voidunit();
    }
    FPlane(){
      name = "plane";
    }
  } fplane;


  class FImageplane: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getany(Image::MyMesh*,scene);
      getany(CImg<unsigned char>*,texture);
      getint(sizex);
      getint(sizey);
      getint(x);
      getint(y);
      getint(z);
      getdouble(opacity);
      close_parameters;
      unique_ptr<Image::MyMesh> imageplane = Image::fmimageplane(sizex, sizey, x, y, z, *texture, opacity);
      cout << "MESH" << imageplane->primitives.size() << endl;
      Image::append(scene, imageplane);
      return voidunit();
    }
    FImageplane(){
      name = "imageplane";
    }
  } fimageplane;

  class FHPlane: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(Image::MyMesh,scene)
      getint(p1);
      getint(p2);
      getint(p3);
      getint(p4);
      getint(p5);
      getint(p6);
      getint(p7);
      getptr(CImg<unsigned char>,p8);
      getdouble(p9);
      close_parameters;
      unique_ptr<Image::MyMesh> plane = Image::fmhplane3d(p1, p2, p3, p4, p5, p6, p7, *p8, p9);
      Image::append(scene, plane);
      return voidunit();
    }
    FHPlane(){
      name = "hplane";
    }
  } fhplane;

  class FShow: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(Image::MyMesh,scene);
      close_parameters;
      scene->show();
      return voidunit();
    }
    FShow(){
      name = "show";
    }
  } fshow;

  class FNewmesh: public Spunk::FValue {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      close_parameters;
      return newmesh(new Image::MyMesh);
    }
    FNewmesh(){
      name = "newmesh";
    }
  } fnewmesh;

  class FLinkpoint: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getany(Image::supportPoint*,point);
      getdouble(x);
      getdouble(y);
      getdouble(z);
      close_parameters;
      return newpoint(point->link(x, y, z));
    }
    FLinkpoint(){
      name = "link";
    }
  } flinkpoint;

  class FTranslatepoint: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getany(Image::supportPoint*,point);
      getdouble(x);
      getdouble(y);
      getdouble(z);
      close_parameters;
      point->translate(x, y, z);
      return voidunit();
    }
    FTranslatepoint(){
      name = "translate";
    }
  } ftranslatepoint;

  class FNewpoint: public Spunk::FValue {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getdouble(x);
      getdouble(y);
      getdouble(z);
      close_parameters;
      return newpoint(new Image::supportPoint(x, y, z));
    }
    FNewpoint(){
      name = "newpoint";
    }
  } fnewpoint;


  class FNewmetaball: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getany(Image::MetaMetaBall*,balls);
      getdouble(radius);
      getdouble(threshold);
      getany(CImg<unsigned char>*,color);
      Image::MetaBall* ball = new Image::MetaBall();
      ball->color = color;
      while(n < parameters.size()){
        getany(Image::supportPoint*, point);
        ball->add(point, radius, threshold);
      }
      close_parameters;
      balls->add(ball);
      return voidunit();
    }
    FNewmetaball(){
      name = "add";
    }
  } fnewmetaball;

  class Fmetaballsmesh: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getany(Image::MetaMetaBall*,balls);
      getdouble(target);
      getdouble(r);
      getint(sub);
      close_parameters;
      Image::MyMesh* mesh = balls->mesh(target, r, sub);
      return newmesh(mesh);
    }
    Fmetaballsmesh(){
      name = "mesh";
    }
  } fmetaballsmesh;

  class FNewmetametaball: public Spunk::FValue {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      close_parameters;
      ValueAny<Image::MetaMetaBall*>* result = new ValueAny<Image::MetaMetaBall*>(new Image::MetaMetaBall, "Meta Meta Ball");
      result->members.push_back(&fnewmetaball);
      result->members.push_back(&fmetaballsmesh);
      return result;
    }
    FNewmetametaball(){
      name = "newmetametaball";
    }
  } fnewmetametaball;


  class FNewimg: public Spunk::FValue {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getint(p0);
      getint(p1);
      getint(p2);
      getint(p3);
      getint(p4);
      close_parameters;
      CImg<unsigned char>* img = new CImg<unsigned char>(p0,p1,p2,p3,p4);
      return newimage(img);
    }
    FNewimg(){
      name = "newimg";
    }
  } fnewimg;

  class Fwait: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(CImgDisplay,disp);
      getint(p1);
      close_parameters;
      disp->wait(p1);
      return voidunit();
    }
    Fwait(){
      name = "wait";
    }
  } fwait;

  class Fclear: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(CImg<unsigned char>,img);
      close_parameters;
      img->fill(0);
      return voidunit();
    }
    Fclear(){
      name = "clear";
    }
  } fclear;

  class Fdisplay: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(CImg<unsigned char>,img);
      getptr(CImgDisplay,disp);
      close_parameters;
      disp->display(*img);
      return voidunit();
    }
    Fdisplay(){
      name = "display";
    }
  } fdisplay;

  class Floadimage: public Spunk::FValue {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getany(string,p0);
      close_parameters;
      cout << "READING " << p0 << endl;;
      CImg<unsigned char>* image = new CImg<unsigned char>(CImg<unsigned char>().load(p0.data()));
      //      cout << "Done " << p0 << " " << image->spectrum() << endl;;
      return newimage(image);
    }
    Floadimage(){
      name = "loadimage";
    }
  } floadimage;

  class Fcopyimage: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(CImg<unsigned char>,img);
      close_parameters;
      CImg<unsigned char>* image = new CImg<unsigned char>(*img);
      return newimage(image);
    }
    Fcopyimage(){
      name = "copy";
    }
  } fcopyimage;


  class Fresize: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(CImg<unsigned char>,img);
      getint(size_x);
      getint(size_y);
      close_parameters;
      CImg<unsigned char>* image;
      if (size_x == -50 && size_y == -50)
        image = new CImg<unsigned char>(img->get_resize_halfXY());
      else
      if (size_x == -200 && size_y == -200)
        image = new CImg<unsigned char>(img->get_resize_doubleXY());
      else
        image = new CImg<unsigned char>(img->get_resize(size_x, size_y));
      return newimage(image);
    }
    Fresize(){
      name = "resize";
    }
  } fresize;

  class Fdrawimage: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(CImg<unsigned char>,img);
      getptr(CImg<unsigned char>,image);
      getint(p2);
      getint(p3);
      getdouble(p4);
      getint(transparent);
      close_parameters;
      //cout << "IMG" << img->size() << " " << image->size() << endl;
      img->draw_image_fm(p2, p3, 0, 0, *image, (float)p4, transparent != 0);
      //img->draw_image(p2, p3, 0, 0, *image, (float)p4);
      return voidunit();
    }
    Fdrawimage(){
      name = "drawimage";
    }
  } fdrawimage;

  class Floadmesh: public Spunk::FValue {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getany(string,path);
      getptr(CImg<unsigned char>,col);
      getdouble(opacity);
      close_parameters;
      cout << "READING " << path << endl;;
      Image::MyMesh* mesh = (new Image::MyMesh())->readOFF(path, *col, opacity);
      //      cout << "Done " << path << endl;;
      return newmesh(mesh);
    }
    Floadmesh(){
      name = "loadmesh";
    }
  } floadmesh;

  class Fcolor_from: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(Image::MyMesh,mesh);
      getdouble(x);
      getdouble(y);
      getdouble(z);
      getdouble(xf);
      getdouble(yf);
      getdouble(zf);
      getdouble(r0);
      getdouble(g0);
      getdouble(b0);
      close_parameters;
      mesh->colorfrom(x, y, z, xf, yf, zf, r0, g0, b0);
      return voidunit();
    }
    Fcolor_from(){
      name = "color_from";
    }
  } fcolor_from;

  class Fshrink_from: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(Image::MyMesh,mesh);
      getdouble(x);
      getdouble(y);
      getdouble(z);
      getdouble(xf);
      getdouble(yf);
      getdouble(zf);
      getdouble(coef);
      close_parameters;
      mesh->shrinkfrom(x, y, z, xf, yf, zf, coef);
      return voidunit();
    }
    Fshrink_from(){
      name = "shrink_from";
    }
  } fshrink_from;


  class Fselect: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(Image::MyMesh,mesh);
      getdouble(x);
      getdouble(y);
      getdouble(z);
      getdouble(xf);
      getdouble(yf);
      getdouble(zf);
      close_parameters;
      mesh->select(x, y, z, xf, yf, zf);
      return voidunit();
    }
    Fselect(){
      name = "select";
    }
  } fselect;

  class Fdrawmesh: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(CImg<unsigned char>,img);
      getptr(Image::MyMesh,mesh);
      getdouble(x);
      getdouble(y);
      getdouble(z);
      getdouble(p2);
      getdouble(p3);
      getdouble(p4);
      getdouble(focale)
      close_parameters;
      mesh->drawrotate(img, x, y, z, p2, p3, p4, focale);
      return voidunit();
    }
    Fdrawmesh(){
      name = "drawmesh";
    }
  } fdrawmesh;

  class Fnewdisplay: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(CImg<unsigned char>,img);
      close_parameters;
      CImgDisplay disp(*img,"Spark");
      auto result = new ValueAny<CImgDisplay*>(new CImgDisplay(disp), "CImgDisplay");
      result->members.push_back(&fwait);
      return result;
    }
    Fnewdisplay(){
      name = "newdisplay";
    }
  } fnewdisplay;

  class Frectangle: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(CImg<unsigned char>,img);
      getint(p1);
      getint(p2);
      getint(p3);
      getint(p4);
      getptr(CImg<unsigned char>,p5);
      getdouble(p6);
      close_parameters;
      img->draw_rectangle(p1, p2, p3, p4, p5->data(), (float)p6);;
      return voidunit();
    }
    Frectangle(){
      name = "rectangle";
    }
  } frectangle;

  class Ftext: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      if(parameters.size() == 7){
	open_parameters;
        getptr(CImg<unsigned char>,img);
        getint(x);
        getint(y);
        getany(string,s);
        getptr(CImg<unsigned char>,color);
        getdouble(opacity);
        getint(size);
	close_parameters;
        img->draw_text(x, y, s.data(), color->data(), 0,(float)opacity, size);
        return voidunit();
      }
      open_parameters;
      getptr(CImg<unsigned char>,img);
      getint(x);
      getint(y);
      getany(string,s);
      getptr(CImg<unsigned char>,color);
      getptr(CImg<unsigned char>,bgcolor);
      getdouble(opacity);
      getint(size);
      close_parameters;
      img->draw_text(x, y, s.data(), color->data(), bgcolor->data(),(float)opacity, size);
      //      img->draw_text(3,3,"Mouse buttons\nto zoom in/out",color,0,0.8f,24)
      return voidunit();
    }
    Ftext(){
      name = "text";
    }
  } ftext;

  class Fsavebmp: public Spunk::member<Value> {
  public:
    Value* eval(vector<Value*>& parameters){
      open_parameters;
      getptr(CImg<unsigned char>,img);
      getint(p1);
      close_parameters;
      //      getptr(CImg<unsigned char>,p5);
      string s = boost::lexical_cast<std::string>(p1);
      string s0 = "000000";
      s0.resize(6 - s.size());
      string filename = "/home/fmaurel/prog/spark/output/image" + s0 + s + ".bmp";
      img->save_bmp(filename.data());
      return voidunit();
    }
    Fsavebmp(){
      name = "savebmp";
    }
  } fsavebmp;

  ValueAny<Image::supportPoint*>* newpoint(Image::supportPoint* point){
    Spunk::ValueAny<Image::supportPoint*>* v = new Spunk::ValueAny<Image::supportPoint*>(point, "point");
    v->members.push_back(&flinkpoint);
    v->members.push_back(&ftranslatepoint);
    return v;
  }

  ValueAny<Image::MyMesh*>* newmesh(Image::MyMesh* point){
    ValueAny<Image::MyMesh*>* v = new ValueAny<Image::MyMesh*>(point, "point");
    v->members.push_back(&fimageplane);
    v->members.push_back(&fplane);
    v->members.push_back(&fhplane);
    v->members.push_back(&fshow);
    v->members.push_back(&fcolor_from);
    v->members.push_back(&fshrink_from);
    v->members.push_back(&fselect);
    return v;
  }

  ValueAny<CImg<unsigned char>*>* newimage(CImg<unsigned char>* point){
    ValueAny<CImg<unsigned char>*>* v = new ValueAny<CImg<unsigned char>*>(point, "image");
    v->members.push_back(&fdrawmesh);
    v->members.push_back(&fsavebmp);
    v->members.push_back(&fdisplay);
    v->members.push_back(&fnewdisplay);
    v->members.push_back(&frectangle);
    v->members.push_back(&ftext);
    v->members.push_back(&fsavebmp);
    v->members.push_back(&fcopyimage);
    v->members.push_back(&fdrawimage);
    v->members.push_back(&fresize);
    v->members.push_back(&fclear);
    return v;
  }

  Env* env(){
    Env* env = newenv();
    env->functions.push_back(&fcolor);
    env->functions.push_back(&fnewimg);
    env->functions.push_back(&floadmesh);
    env->functions.push_back(&fnewmesh);
    env->functions.push_back(&floadimage);
    env->functions.push_back(&fnewpoint);
    env->functions.push_back(&fnewmetametaball);
    return env;
  }
}

int main(int argc, char **argv) {
  auto env = Spark::env();

  here;
  unique_ptr<Spunk::Expr> e = Spunk::load(argc, argv);
  e->eval(*env);
  return 0;
}
