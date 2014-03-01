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
//#include <dirent.h>

//#include <boost/filesystem/operations.hpp>
//#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>

#ifndef cimg_imagepath
#define cimg_imagepath "../CImg/CImg-1.5.7/examples/img/"
#endif
#define faileval {cout << "Line: " << __LINE__ << endl; throw new EvalException;}
#define failparse {cout << "Line: " << __LINE__ << " " << nextPos << endl; throw new ParseException;}
#define ccout(s) {cout << "Line: " << __LINE__ << " NextPos: " << nextPos << " " << s << endl; }
#define here //{cout << "Here: " << __LINE__ << endl;}

#define check_parameters(siz) if (parameters.size() != siz) faileval;int n = 0;
//#define getptr(t,tt,v) if(parameters[n]->kind != Ptr || parameters[n]->ptr_kind != tt) faileval;t* v = (t*)parameters[n]->ptr;n++;
#define getany(any,v) ValueAny<any>* v ## ptr = dynamic_cast<ValueAny<any>*>(parameters[n]); if (v ## ptr == 0) faileval; any v = v ## ptr->value; n++;
#define getptr(any,t,v) getany(any*,v)
#define getdouble(v) ValueAny<double>* v ## ddd = doubleValue(parameters[n]); if (v ## ddd == 0) faileval; double v = v ## ddd->value; n++;
#define getint(v) ValueAny<int>* v ## ddd = intValue(parameters[n]); if (v ## ddd == 0) faileval; int v = v ## ddd->value; n++;

//#define getint(v) ValueAny<int>* v ## ptr = dynamic_cast<ValueAny<int>*>(parameters[n]); if (v ## ptr == 0) faileval; int v = v ## ptr->value; n++;

//if(!parameters[n]->isNum) faileval; int v = parameters[n]->intValue;n++;
//#define getdouble(v) if(!parameters[n]->isNum) faileval; double v = parameters[n]->doubleValue;n++;
//#define get(string,v) if(parameters[n]->kind != String) faileval; string v = parameters[n]->stringValue;n++;

using namespace std;
using namespace cimg_library;
//using namespace boost::filesystem; 


void handler(int sig) {
  
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  //string[] messages = backtrace_symbols(array, size);
  
  //exit(1);
}



namespace Image
{
  // bool loadOBJ(
  // 	       const char * path,
  // 	       std::vector < glm::vec3 > & out_vertices,
  // 	       std::vector < glm::vec2 > & out_uvs,
  // 	       std::vector < glm::vec3 > & out_normals
  // 	       )
  // {	
  //   std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  //   std::vector< glm::vec3 > temp_vertices;
  //   std::vector< glm::vec2 > temp_uvs;
  //   std::vector< glm::vec3 > temp_normals;
	
  //   FILE * file = fopen(path, "r");
  //   if( file == NULL ){
  //     printf("Impossible to open the file !\n");
  //     return false;
  //   }
	
  //   while( 1 ){
 
  //     char lineHeader[128];
  //     // read the first word of the line
  //     int res = fscanf(file, "%s", lineHeader);
  //     if (res == EOF)
  // 	break; // EOF = End Of File. Quit the loop.
 
  //     // else : parse lineHeader
	
  //     if ( strcmp( lineHeader, "v" ) == 0 ){
  // 	glm::vec3 vertex;
  // 	fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
  // 	temp_vertices.push_back(vertex);
	
  //     }else if ( strcmp( lineHeader, "vt" ) == 0 ){
  // 	glm::vec2 uv;
  // 	fscanf(file, "%f %f\n", &uv.x, &uv.y );
  // 	temp_uvs.push_back(uv);
	
  //     }else if ( strcmp( lineHeader, "vn" ) == 0 ){
  // 	glm::vec3 normal;
  // 	fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
  // 	temp_normals.push_back(normal);
  //     }else if ( strcmp( lineHeader, "f" ) == 0 ){
  // 	std::string vertex1, vertex2, vertex3;
  // 	unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
  // 	int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
  // 	if (matches != 9){
  // 	  printf("File can't be read by our simple parser : ( Try exporting with other options\n");
  // 	  return false;
  // 	}
  // 	vertexIndices.push_back(vertexIndex[0]);
  // 	vertexIndices.push_back(vertexIndex[1]);
  // 	vertexIndices.push_back(vertexIndex[2]);
  // 	uvIndices    .push_back(uvIndex[0]);
  // 	uvIndices    .push_back(uvIndex[1]);
  // 	uvIndices    .push_back(uvIndex[2]);
  // 	normalIndices.push_back(normalIndex[0]);
  // 	normalIndices.push_back(normalIndex[1]);
  // 	normalIndices.push_back(normalIndex[2]);
	
  // 	// For each vertex of each triangle
  // 	for( unsigned int i=0; i<vertexIndices.size(); i++ ){
	
  // 	  unsigned int vertexIndex = vertexIndices[i];
	
  // 	  glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
	
  // 	  out_vertices.push_back(vertex);
	
  // 	  // Read our .obj file
  // 	  std::vector< glm::vec3 > vertices;
  // 	  std::vector< glm::vec2 > uvs;
  // 	  std::vector< glm::vec3 > normals; // Won't be used at the moment.
  // 	  bool res = loadOBJ("cube.obj", vertices, uvs, normals);
	
  // 	  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
  // 	}
  //     }
  //   }

  void plot2d(int argc, char **argv){
    const char* expression = "2 * x";
    const char *const formula = cimg_option("-f",expression,"Formula to plot");
    const float x0 = cimg_option("-x0", -5.0f, "Minimal X-value");
    const float x1 = cimg_option("-x1", 5.0f, "Maximal X-value");
    const int resolution = cimg_option("-r",1024,"Plot resolution");
    const unsigned int nresolution = resolution > 1 ? resolution:1024;
    const unsigned int plot_type = cimg_option("-p", 1, "Plot type");
    const unsigned int vertex_type = cimg_option("-v", 1, "Vertex type");

    // Create plot data.
    CImg<double> values(4,nresolution,1,1,0);
    const unsigned int r = nresolution - 1;
    cimg_forY(values,X) values(0,X) = x0 + X*(x1-x0)/r;
    cimg::eval(formula,values).move_to(values);

    // Display interactive plot window.
    CImg<double> graph = values.display_graph(formula,plot_type,vertex_type,"X-axis",x0,x1,"Y-axis");
    graph.save_bmp("/home/fmaurel/prog/spark/test1.bmp");
  }

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


  // struct fm3d {
  //   CImgList<unsigned int> prims;
  //   CImg<float> pts;
  //   CImgList<unsigned char> cols;
  //   CImg<float> opacities;
  // };

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
	//vertex(x + 1000, -y+1000, -z+1000);
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
    // void draw(CImg<unsigned char>* img){
    //   CImg<unsigned char> col = CImg<unsigned char>::vector(0, 255, 0);
    //   CImgList<unsigned char> cols = CImgList<unsigned char>(primitives.size(), col);
    //   CImg<float> opacities = CImg<float>(primitives.size(), 0.5f);
    //   img->draw_object3d(0, 0, 0, vertices>'x', primitives, cols, opacities);
    // }
    void drawrotate(CImg<unsigned char>* img, int x, int y, int z, float alpha, float beta, float gamma, float focale){
      //      cout << "draw1 " << vertices.size() << endl;
      //      const CImg<float>	rpoints = CImg<>::rotation_matrix(1,1,0,(alpha))*CImg<>::rotation_matrix(1,0,1,(beta))*CImg<>::rotation_matrix(0,1,1,(gamma))*((vertices>'x').shift_object3d());
      const CImg<float>	rpoints = CImg<>::rotation_matrix(1,0,0,(alpha))*CImg<>::rotation_matrix(0,1,0,(beta))*CImg<>::rotation_matrix(0,0,1,(gamma))*((vertices));
      //      cout << "BB" << endl;
      //cout << "draw2 " << rpoints.size() << " " << endl;
      img->draw_object3d(x, y, z, rpoints, primitives, colors, opacities, 4, false, focale);
      //cout << "draw3" << endl;
      //      cout << "CC" << endl;
    }
  };

  void show(MyMesh* scene){
    const CImg<unsigned char> visu = CImg<unsigned char>(3,512,512,1).fill(230,230,255).permute_axes("yzcx");
    CImg<float> view_matrix = CImg<>::identity_matrix(4);
    visu.display_object3d("SP 1",scene->vertices,scene->primitives,scene->colors,scene->opacities,true,4,4,false,
			  500.0f,0,0,-5000,0.5f,0.1f,true,view_matrix.data());
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
    //  scene->colors.insert(other->colors);
    scene->colors=(+scene->colors, other->colors);
  
    //scene->opacities = 
    //  scene->opacities.append(other->opacities);
    scene->opacities=(scene->opacities, other->opacities)>'x';
  }

  class Metaball {
  public:
    float x, y, z, r, threshold;
    Metaball(float x, float y, float z, float r, float threshold){
      this->x = x;
      this->y = y;
      this->z = z;
      this->r = r;
      this->threshold = threshold;
    }
  };

  class Metaballs {
  private:
    vector<Metaball*> balls;
  public:
    void add(float x, float y, float z, float r, float threshold){
      balls.push_back(new Metaball(x, y, z, r, threshold));
    }
    inline float operator()(const float x, const float y, const float z) const {
      float potential = 0;
      for(unsigned int i = 0; i < balls.size(); i++){
	float dx = x - balls[i]->x;
	float dy = y - balls[i]->y;
	float dz = z - balls[i]->z;
	float r = (dx * dx + dy * dy + dz * dz) / balls[i]->r;
	float v = 1 / (r);
	//	if (r < balls[i]->threshold && ( potential == 0 || potential < v))
	if (r < balls[i]->threshold)
	  potential += v;//(1 - r * r) * (1 - r * r);
	  //	  potential += 1 - r * (r * (4 * r + 17) - 22) / 9;
      }
      return potential;
    }
    MyMesh* mesh(float target, float r, int sub){
      MyMesh* result = new MyMesh();
      result->vertices = CImg<>::isosurface3d(result->primitives,*this,target,-r,-r,-r,r,r,r,sub);
      result->primitives.reverse_object3d();
      return result;
    }
  };
}

namespace ExpParser
{
  class ParseException: public exception
  {
    public:
    //    ParseException(string message){this.message = message;}
    virtual const char* what() const throw()
    {
      return "My exception happened";
    }
  };

  class EvalException: public exception
  {
    public:
    //    ParseException(string message){this.message = message;}
    virtual const char* what() const throw()
    {
      return "My exception happened";
    }
  };

  enum tokenType { Ident, StringCst, End, Op, Unknown, Keyword };

  struct token {
    int pos;
    int nextPos;
    string s;
    tokenType type;
  };

  string ops = "+-*/=";
  string intchars = "-0123456789";
  string doublechars = "-0123456789.";

  bool stringContains(string& s, char c){
    for(unsigned int i = 0; i < s.length(); i++)
      if(c == s[i])
	return true;
    return false;
  }

  bool isint(string& s){
    for(unsigned int i = 0; i < s.length(); i++)
      //      if(!isalnum(s[i]))
      if (!stringContains(intchars, s[i]))
	return false;
    return true;
  }

  bool isOp(char c){
    return stringContains(ops, c);
  }

  string spaces = " \t\n\r";

  bool isSpace(char c){
    for(unsigned int i = 0; i < spaces.length(); i++)
      if(c == spaces[i])
	return true;
    return false;
  }

  bool isLetter(char c){
    return isalpha(c) || isalnum(c) || c == '_';
  }

  int identToken(string s, unsigned int pos){
    int len = 0;
    if (s.length() <= pos)
      return len;
    if(stringContains(intchars, s[pos]))
      while(pos < s.length() && stringContains(doublechars, s[pos + len]))
	len++;
    else
      while(pos < s.length() && isLetter(s[pos + len]))
	len++;
    return len;
  }

  bool HasToken(string s, unsigned int pos){
    int length = s.length();
    if (0 == length)
      return false;
    unsigned int maxpos = s.length() - 1;
    while(pos <= maxpos){
      if (pos < maxpos - 1 && s[pos] == '/' && s[pos + 1] == '/')
	while(pos <= maxpos && s[pos] != '\n')
	  pos++;
      else if (!isSpace(s[pos]))
	return true;
      pos++;
    }
    return false;      
  }

  unique_ptr<token> NextToken(string s, unsigned int pos){
    unique_ptr<token> tok(new token);
    int len = 0;
    tokenType type = Unknown;
    bool done = false;

    while(!done){
      if (s.length() <= pos)
	{
	  cout << pos << endl;
	  handler(1);
	  throw new exception();
	  type = End;
	  break;
	}
      done = true;
      switch(s[pos]){
      case '{':
      case '}':
      case '.':
      case ';':
      case '(':
      case ',':
      case ')':
	len = 1;
	type = Keyword;
	break;
      case ' ':
      case '\t':
      case '\n':
      case '\r':
	pos++;
	done = false;
	break;
      case '"':
	{
	  //	  int nextPos = -12;
	  //failparse;
	  len = 1;
	  while(pos + len < s.length() && s[pos + len] != '"')
	    len++;
	  len++;
	  type = StringCst;
	}
	break;
      case '/':
	if (pos < s.length() - 2 && s[pos + 1] == '/'){
	  while(pos < s.length() && s[pos] != '\n')
	    pos++;
	  done = false;
	}
	else{
	  len = 1;
	  type = Op;
	}
	break;
      default:
	if(isOp(s[pos])){
	  len = 1;
	  type = Op;
	}
	else{
	  len = identToken(s, pos);
	  type = Ident;
	}      
	break;
      }
    }

    tok->pos = pos;
    if (type != End)
      tok->s = s.substr(pos, len);
    tok->type = type;
    tok->nextPos = pos + len;

    if (type == StringCst){
      if (type != End)
	tok->s = s.substr(pos + 1, len - 2);
      //cout << "STRING " << pos << " " << len << " " << tok->s << endl;
    }


    return tok;
  }

  bool HasToken(string& s, int& pos, string v){
    if (!HasToken(s, pos))
      return false;
    int nextPos = pos;
    unique_ptr<token> tok = NextToken(s, nextPos);
    if (tok->s != v)
      return false;
    pos = tok->nextPos;
    return true;
  }

  enum Kind { Int, Double, String, Function, Ptr };

  class Expr;

  class Value {
  public:
    virtual string tostring() = 0;
    virtual int get_int() = 0;
  };

  template<typename T>
  class ValueAny: public Value {
  public:
    T value;
    string s;
  public:
    Kind kind;
    ValueAny(T v, string pp){
      value = v;
      s = pp;
    }
    virtual string tostring(){
      return s;
    }
    virtual int get_int(){
      return 0;
    }
  };

  template<>
  class ValueAny<int>: public Value {
  public:
    int value;
    string s;
  public:
    Kind kind;
    ValueAny(int v, string pp){
      value = v;
      s = pp;
    }
    ValueAny(int v){
      char output[1024];
      sprintf(output, "%i", v);
      value = v;
      s = output;
    }
    virtual string tostring(){
      return s;
    }
    virtual int get_int(){
      return value;
    }
  };

  template<>
  class ValueAny<double>: public Value {
  public:
    double value;
    string s;
  public:
    Kind kind;
    ValueAny(double v, string pp){
      value = v;
      s = pp;
    }
    ValueAny(double v){
      char output[1024];
      sprintf(output, "%g", v);
      value = v;
      s = output;
    }
    virtual string tostring(){
      return s;
    }
    virtual int get_int(){
      return (int) value;
    }
  };

  class ValueF: public Value {
  public:
    unique_ptr<Expr> funcValue;
    vector<string> parameters;
    ValueF(unique_ptr<Expr>& f, vector<string> params){
      funcValue = move(f);
      parameters = params;
    }
    string tostring(){
      return "<fun>";
    }
    virtual int get_int(){
      faileval;
    }
  };
 
  // template<typename T>
  // class ValueFM: public Value {
  // public:
  //   bool isNum;
  //   int intValue;
  //   double doubleValue;
  //   string stringValue;
  //   unique_ptr<Expr> funcValue;
  //   vector<string> parameters;
  //   void* ptr;
  //   string ptr_kind;
  // public:
  //   Kind kind;
  //   ValueFM(int v){
  //     isNum = true;
  //     kind = Int;
  //     doubleValue = v;
  //     intValue = v;
  //   }
  //   ValueFM(double v){
  //     isNum = true;
  //     kind = Double;
  //     intValue = (int)v;
  //     doubleValue = v;
  //   }
  //   ValueFM(string v){
  //     isNum = false;
  //     kind = String;
  //     stringValue = v;
  //   }
  //   ValueFM(unique_ptr<Expr>& f, vector<string> params){
  //     isNum = false;
  //     kind = Function;
  //     funcValue = move(f);
  //     parameters = params;
  //   }
  //   ValueFM(void* v, string k){
  //     isNum = false;
  //     kind = Ptr;
  //     ptr_kind = k;
  //     ptr = v;
  //   }
  //   string tostring(){
  //     char output[1024];
  //     switch(kind){
  //     case Int:
  //       sprintf(output, "%i", intValue);
  // 	//cout << output << " !!" << endl;
  // 	return output;
  // 	break;
  //     case Double:
  //       sprintf(output, "%g", doubleValue);
  // 	return output;
  // 	break;
  //     case String:
  // 	return stringValue;
  // 	break;
  //     case Function:
  // 	return "<fun>";
  // 	break;
  //     // case Cimg_char:
  //     // 	return "< ?? >";
  //     // 	break;
  //     case Ptr:
  // 	return "<" + ptr_kind + ">";
  // 	break;
  //     }
  //     cout << "Kind" << kind << endl;
  //     faileval;
  //   }
  // };

  Value* unit = new ValueAny<int>(0, "unit");

  class FValue {
  public:
    string name;
    virtual Value* eval(vector<Value*> parameters)=0;
  };

  class Env {
  public:
    vector<string> names;
    vector<Value*> values;
    vector<FValue*> functions;
    void add(string name, Value* value){
      names.push_back(name);
      values.push_back(value);
    }
  };

  template<typename T1, typename T2>
  unique_ptr<T2>* assoc(vector<T1>names, vector<unique_ptr<T2>> values, T1 name){
    for(int i = 0; i < names.size(); i++)
      if (names[i] == name)
	return &values[i];
    return 0;
  }

  enum ExprKind { ExFunc };

  class Func;

  class Expr {
  public:
    ExprKind kind;
    virtual unique_ptr<Expr> copy()=0;
    virtual unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values)=0;
    virtual unique_ptr<Func> FuncMe()=0;
    virtual Value* eval(Env& env)=0;
    virtual string tostring()=0;
  };

  class StringExpr: public Expr {
  public:
    string v;
    StringExpr(string s){
      v = s;
    }
    unique_ptr<Expr> copy(){return unique_ptr<Expr>(new StringExpr(v));}
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    virtual unique_ptr<Expr> substitute(vector<string>&, vector<unique_ptr<Expr>>&){
      unique_ptr<Expr> result(this);
      return result;
    }
    virtual Value* eval(Env&){
      return new ValueAny<string>(v, v);
    }
    virtual string tostring(){
      return v;
    }
  };

  class Func: public Expr {
  public:
    string name;
    bool isFunction;
    bool isInfix;
    vector<unique_ptr<Expr>> parameters;
    bool isValue;
    //Value* v;
    unique_ptr<Func> innerCopy(){
      here;
      unique_ptr<Func> result (new Func);
      result->name = name;
      result->isFunction = isFunction;
      result->isInfix = isInfix;
      for(unsigned int i = 0; i < parameters.size(); i++)
	result->parameters.push_back(parameters[i]->copy());
      result->isValue = isValue;
      here;
      return move(result);
    }
    unique_ptr<Expr> copy(){
      return move(innerCopy());
    }
    Func(){ isFunction=false; isInfix=false; isValue=false;kind=ExFunc; }
    unique_ptr<Expr> applySubstitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      here;
      unique_ptr<Func> result(new Func);
      result->name = name;
      result->isFunction = isFunction;
      result->isInfix = isInfix;
      for(unsigned int i = 0; i < parameters.size(); i++)
	result->parameters.push_back(parameters[i]->substitute(names, values));
      return move(result);
    }

    unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      here;
      //unique_ptr<Expr>* v;// = assoc<string, Expr>(names, values, name));
      int id = -1;
      for(unsigned int i = 0; i < names.size(); i++)
	if (names[i] == name)
	  id = i;
      //v = &(values[i]);
      here;
      if (id < 0){
	here;
	if (parameters.size() == 0)
	  return copy();
	return applySubstitute(names, values);
      }
      here;
      unique_ptr<Expr> v = values[id]->copy();
      here;
      //cout << "SUB " << name << " " << parameters.size() << " " << tostring() << " " << v->tostring() <<endl;
      if (!isFunction)
	return v->copy();
      else{
	here;
	if (v->kind != ExFunc)
	  faileval;
	here;
	//Func v = v.get();
	//unique_ptr<Func> vv ((Func*)(&(v*)));
	unique_ptr<Func> vv = v->FuncMe();
	// cout << tostring() << endl;
	// cout << v->tostring() << " " << typeid(*v).name() << endl;
	// cout << vv->name << " " << vv->isFunction << " " << vv->parameters.size() << " " << (vv->isValue?"true":"false") << endl;
	if (vv->isFunction || vv->isValue)
	  faileval;
	here;
	unique_ptr<Func> result = applySubstitute(names, values)->FuncMe();
	result->name = vv->name;
	return move(result);
      }
    }

    virtual unique_ptr<Func> FuncMe(){
      return innerCopy();
    }
    Value* eval(Env& env){
      here;
      //cout << name << endl;
      if (isValue)
	faileval;
      //return v;
      if (isFunction) {
	if (name.length() == 0 && parameters.size() == 1)
	  return parameters[0]->eval(env);
	here;
	for(unsigned int i = 0; i < env.functions.size(); i++)
	  if (name == env.functions[i]->name){
	    FValue* f = env.functions[i];
	    vector<Value*> params;
	    for(unsigned int j = 0; j < parameters.size(); j++)
	      params.push_back(parameters[j]->eval(env));
	    return f->eval(params);
	  }
	here;
	for(int i = env.names.size() -1; 0 <= i; i--)
	  if (name == env.names[i]){
	    ValueF* f = dynamic_cast<ValueF*>(env.values[i]);
	    if (f == 0)
	      faileval;
	    //if (f->funcValue
	    //vector<Value*> params;
	    //	    for(unsigned int j = 0; j < parameters.size(); j++)
	    // params.push_back(parameters[j]);
	    if (f->parameters.size() != parameters.size()){
	      cout << "Bad nb parameters for application. " << tostring() << endl;
	      faileval;
	    }
	    here;
	    //cout << f->funcValue->tostring() << endl;
	    unique_ptr<Expr> e = f->funcValue->substitute(f->parameters, parameters);
	    here;
	    return e->eval(env);
	    //int length = 
	    faileval;
	  }

	cout << "Unknown function: " << name << " ";
	faileval
      }
      here;
      for(int i = env.names.size() -1; 0 <= i; i--)
	if (name == env.names[i])
	  return env.values[i];
      if (stringContains(name, '.')){ // TODO isdouble
	float res = 0.;
	char* nameChars = (char*)(name.data());
	sscanf(nameChars, "%g", &res);
	return new ValueAny<double>((double)res, name);
      } else if (isint(name)){
	int res = 0;
	char* nameChars = (char*)(name.data());
	sscanf(nameChars, "%i", &res);
	return new ValueAny<int>(res, name);
      }
      cout << "Unknown variable: " + name << endl;
      faileval;
    }
    string tostring() {
      string result;
      if (isInfix){
	unsigned int size = parameters.size();
	for(unsigned int i = 0; i < size; i++){
	  result += parameters[i]->tostring();
	  if (i < size - 1)
	    result += " " + name + " ";
	}
	return result;
      }
      result = name;
      if (isFunction)
	result += "(";
      unsigned int size = parameters.size();
      for(unsigned int i = 0; i < size; i++)
	{
	  result += parameters[i]->tostring();
	  if (i < size - 1)
	    result += ", ";
	}
      if (isFunction)
	result += ")";
      return result;
    }
  };

  // class Command {
  // public:
  //   virtual void eval(Env& env)=0;
  //   virtual string tostring()=0;
  // };

#define new_result(t) unique_ptr<t> result(new t);

  class Definition: public Expr {
  public:
    bool isFunction;
    string name;
    vector<string> parameters;
    unique_ptr<Expr> value;
    Definition(){isFunction=false;}
    unique_ptr<Expr> copy(){
      new_result(Definition);
      result->isFunction = isFunction;
      result->name = name;
      result->parameters = vector<string>(parameters);
      result->value = value->copy();
      return move(result);
    }
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      vector<string> subnames;
      vector<unique_ptr<Expr>> subvalues;
      for(unsigned int i = 0; i < names.size(); i++)
	if (names[i] != name){
	  subnames.push_back(names[i]);
	  subvalues.push_back(values[i]->copy());
	}
      unique_ptr<Expr> v = value->substitute(subnames, subvalues);
      unique_ptr<Definition> result(new Definition);
      result->isFunction = isFunction;
      result->name = name;
      result->parameters = parameters;
      result->value = move(v);
      return move(result);
    }
    virtual string tostring() { return "var " + name + " = " + value->tostring() + ";"; }
    virtual Value* eval(Env& env){
      env.names.push_back(name);
      if (isFunction)
	env.values.push_back(new ValueF(value, parameters));
      else
	env.values.push_back(value->eval(env));
      return unit;
    }
  };

  class Assign: public Expr {
  public:
    string name;
    unique_ptr<Expr> value;
    unique_ptr<Expr> copy(){
      new_result(Assign);
      result->name = name;
      result->value = value->copy();
      return move(result);
    }
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    virtual unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      unique_ptr<Assign> result(new Assign);
      result->name = name;
      result->value = value->substitute(names, values);
      return move(result);
    }

    virtual string tostring() { return "set " + name + " = " + value->tostring() + ";"; }
    virtual Value* eval(Env& env){
      //Value* v = value.eval(env);
      //cout << name << " <- " << v->tostring() << endl;
      for(int i = env.names.size() -1; 0 <= i; i--)
	if (name == env.names[i]){
	  env.values[i] = value->eval(env);
	  return unit;
	}
      cout << "Unknown variable: " + name << endl;
      faileval;
    }
  };

  // class Call: public Expr {
  // public:
  //   Func f;
  //   virtual string tostring() {
  //     return f.tostring() + ";";  
  //   }
  //   virtual Value* eval(Env& env){
  //     //cout << "ENV" << env.names.size() << endl;
  //     f.eval(env);
  //     //  int length = env.names.size();
  //     //for(unsigned int i = 0; i < commands.size(); i++)
  //     //	commands[i]->eval(env);
  //     //env.resize(length);
  //   }
  // };

  class Block: public Expr {
  public:
    vector<unique_ptr<Expr>> commands;
    unique_ptr<Expr> copy(){
      new_result(Block);
      for(unsigned int i = 0; i < commands.size(); i++)
	result->commands.push_back(commands[i]->copy());
      return move(result);
    }
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      here;
      unique_ptr<Block> b(new Block);
      //cout << "SIZE " << commands.size() << endl;
      for(unsigned int i = 0; i < commands.size(); i++){
	here;
	//cout << commands[i]->tostring() << endl;
	// here;
	// cout << commands[i]->tostring() << endl;
	// here;
	// cout << commands[i]->substitute(names, values)->tostring() << endl;
	// here;
	// cout << commands[i]->tostring() << endl;
	// here;
	// cout << commands[i]->substitute(names, values)->tostring() << endl;
	// here;
	// cout << commands[i]->substitute(names, values)->tostring() << endl;
	// here;
	b->commands.push_back(commands[i]->substitute(names, values));
	here;
      }
      here;
      return move(b);
    }
    virtual string tostring() {
      string s = "{\n";
      for(unsigned int i = 0; i < commands.size(); i++)
	s += commands[i]->tostring() + "\n";
      s += "}\n";
      return s;
    }
    virtual Value* eval(Env& env){
      here;
      int length = env.names.size();
      here;
      for(unsigned int i = 0; i < commands.size(); i++)
	commands[i]->eval(env);
      //cout << env.names.size() << endl;
      //cout << env.values.size() << endl;
      env.names.resize(length);
      env.values.resize(length);
      here;
      return unit;
    }
  };

  string vectorinttype = "vector<int>";
  string cimguchar = "CImg<unsigned char>";
  string mymeshtype = "MyMesh";

  class For: public Expr {
  public:
    string variable;
    unique_ptr<Expr> seq;
    unique_ptr<Expr> command;
    unique_ptr<Expr> copy(){
      new_result(For);
      result->variable = variable;
      result->seq = seq->copy();
      result->command = command->copy();
      return move(result);
    }
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      unique_ptr<For> result(new For);
      vector<string> subnames;
      vector<unique_ptr<Expr>> subvalues;
      for(unsigned int i = 0; i < names.size(); i++)
	if (names[i] != variable){
	  subnames.push_back(names[i]);
	  subvalues.push_back(values[i]->copy());
	}
      result->variable = variable;
      result->seq = seq->substitute(subnames, subvalues);
      result->command = command->substitute(subnames, subvalues);
      return move(result);
    }
    virtual string tostring() {
      return "for("+variable+" in "+seq->tostring()+") "+command->tostring();
    }
    virtual Value* eval(Env& env){
      //      Value* seqvalue = seq->eval(env);
      ValueAny<vector<Value*>*>* seqvalue = dynamic_cast<ValueAny<vector<Value*>*>*>(seq->eval(env));
      if (seqvalue == 0)
	faileval;
      vector<Value*>* vect = seqvalue->value;
      int length = env.names.size();
      env.names.push_back(variable);
      env.values.push_back(unit);
      for(unsigned int i = 0; i < vect->size(); i++)
	{
	  env.values[length] = (*vect)[i];
	  command->eval(env);
	}
      env.names.resize(length);
      env.values.resize(length);
      return unit;
    }
  };

  class While: public Expr {
  public:
    unique_ptr<Expr> cond;
    unique_ptr<Expr> command;
    unique_ptr<Expr> copy(){
      new_result(While);
      result->cond = cond->copy();
      result->command = command->copy();
      return move(result);
    }
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      unique_ptr<While> result(new While);
      result->cond = cond->substitute(names, values);
      result->command = command->substitute(names, values);
      return move(result);
    }
    virtual string tostring() {
      return "while("+cond->tostring()+") "+command->tostring();
    }
    virtual Value* eval(Env& env){
      while(cond->eval(env)->get_int() != 0){
	int length = env.names.size();
	command->eval(env);
	env.names.resize(length);
	env.values.resize(length);
      }
      return unit;
    }
  };

  class Ifte: public Expr {
  public:
    unique_ptr<Expr> cond;
    unique_ptr<Expr> commandt;
    unique_ptr<Expr> commande;
    unique_ptr<Expr> copy(){
      new_result(Ifte);
      result->cond = cond->copy();
      result->commandt = commandt->copy();
      result->commande = commande->copy();
      return move(result);
    }
    virtual unique_ptr<Func> FuncMe(){ faileval; }
    unique_ptr<Expr> substitute(vector<string>& names, vector<unique_ptr<Expr>>& values){
      unique_ptr<Ifte>result(new Ifte);
      result->cond = cond->substitute(names, values);
      result->commandt = commandt->substitute(names, values);
      result->commande = commande->substitute(names, values);
      return move(result);
    }
    virtual string tostring() {
      return "if("+cond->tostring()+") "+commandt->tostring() + " else " + commande->tostring();
    }
    virtual Value* eval(Env& env){
      if(cond->eval(env)->get_int() != 0)
	return commandt->eval(env);
      else
	return commande->eval(env);
    }
  };

  class Parser {
  public:
    virtual unique_ptr<Expr> parse(string s, int& nextPos) = 0;
  };

  class ParserUnaryOps: public Parser{
    string ops;
    Parser* rec;
  public:
    ParserUnaryOps(string ops, Parser* rec){this->ops = ops; this->rec = rec;}
    unique_ptr<Expr> parse(string s, int& nextPos){
      if (HasToken(s, nextPos, "-")){
	if (!HasToken(s, nextPos))
	  failparse;
	unique_ptr<Expr> f1 = rec->parse(s, nextPos);
	unique_ptr<Func> result(new Func);
	result->name = "-";
	result->isFunction = true;
	result->parameters.push_back(move(f1));
	return move(result);
      }
      return rec->parse(s, nextPos);
    }
  };


  class ParserBinOps: public Parser{
    string ops;
    Parser* rec;
  public:
    ParserBinOps(string ops, Parser* rec){this->ops = ops; this->rec = rec;}
    unique_ptr<Expr> parse(string s, int& nextPos){
      unique_ptr<Expr> f1 = rec->parse(s, nextPos);
      while(true){
	if (!HasToken(s, nextPos))
	  return f1;
	int innerNextPos = nextPos;
	unique_ptr<token> tok = NextToken(s, innerNextPos);
	if (tok->type == Op && tok->s.length() == 1 && ops.find(tok->s) != string::npos){
	  innerNextPos = tok->nextPos;
	  unique_ptr<Expr> f2 = rec->parse(s, innerNextPos);
	  unique_ptr<Func> result(new Func);
	  result->isInfix = true;
	  result->isFunction = true;
	  result->name = tok->s;
	  nextPos = innerNextPos;
	  result->parameters.push_back(move(f1));
	  result->parameters.push_back(move(f2));
	  f1 = move(result);
	}
	else
	  break;
      }
      return f1;
    }
  };

  bool stackNotCommaClosePar(string s, int& nextPos, vector<unique_ptr<Expr>>& list);
  unique_ptr<Expr> parseapp(string s, int& nextPos);

  class ParserApp: public Parser {
  public:
    virtual unique_ptr<Expr> parse(string s, int& nextPos){
      return parseapp(s, nextPos);
    }
  };

  Parser* parseApp = new ParserApp;
  Parser* parseTimes = new ParserBinOps("*/", parseApp);
  Parser* parsePlus = new ParserBinOps("+-", parseTimes);
  Parser* parseUMinus = new ParserUnaryOps("-", parsePlus);

  unique_ptr<Expr> parse(string s, int& nextPos){
    return parseUMinus->parse(s, nextPos);
  }

  unique_ptr<Expr> parse(string s){
    int nextPos = 0;
    return parse(s, nextPos);
  }

  unique_ptr<string> getvar(string s, int& nextPos){
    int innerPos = nextPos;
    if (!HasToken(s, innerPos))
      return 0;
    unique_ptr<token> tokVar = NextToken(s, innerPos);
    if (tokVar->type != Ident)
      return 0; 
    nextPos = tokVar->nextPos;
    return unique_ptr<string>(new string(tokVar->s));
  }

  unique_ptr<Expr> parseapp(string s, int& nextPos){
    //result;
    if (HasToken(s, nextPos, "(")){
      unique_ptr<Expr> recPar;
      unique_ptr<Func> result(new Func);
      recPar = parse(s, nextPos);
      if (!HasToken(s, nextPos, ")"))
	faileval;
      result->parameters.push_back(move(recPar));
      result->isFunction = true;
      return move(result);
    }
    unique_ptr<token> tok = NextToken(s, nextPos);
    switch(tok->type){
    case End:
      return 0;
      break;
    case Ident:
      {
	unique_ptr<Func> result(new Func);
	result->name = tok->s;
	nextPos = tok->nextPos;
	if (!HasToken(s, tok->nextPos))
	  return move(result);
	if (HasToken(s, nextPos, "(")){
	  result->isFunction = true;
	  //int innerNextPos = nextPos;
	  while(stackNotCommaClosePar(s, nextPos, result->parameters));
	  //	nextPos = innerNextPos;
	}else if (HasToken(s, nextPos, ".")){
	  unique_ptr<string> f = getvar(s, nextPos);
	  if (f == 0)
	    failparse;
	  result->isFunction = true;
	  unique_ptr<Func> inside(new Func);
	  inside->name = tok->s;
	  result->parameters.push_back(move(inside));
	  result->name = *f;
	  if (!HasToken(s, nextPos, "("))
	    failparse;
	  //int innerNextPos = nextPos;
	  while(stackNotCommaClosePar(s, nextPos, result->parameters));
	  //nextPos = innerNextPos;
	}
	return move(result);
      }
      break;
    case StringCst:
      nextPos = tok->nextPos;
      return unique_ptr<Expr>(new StringExpr(tok->s));
    case Op:
      failparse;
    case Keyword:
      failparse;
    case Unknown:
      cout << endl << "Unknown token at pos " << nextPos << s[nextPos] << endl;
      failparse;
    }
    return 0;
  }

  bool stackNotCommaClosePar(string s, int& nextPos, vector<unique_ptr<Expr>>& list){
    if (HasToken(s, nextPos, ")"))
      return false;
    list.push_back(parse(s, nextPos));
    if (HasToken(s, nextPos, ")"))
      return false;
    else if (HasToken(s, nextPos, ","))
      return true;
    return false;
  }

  unique_ptr<Block> parseCommands(string s, int& pos);

  //Block* nop = new Block;
  unique_ptr<Expr> parseCommand(string s, int& nextPos){
    if (!HasToken(s, nextPos))
      return unique_ptr<Expr>(new Block);
    if (HasToken(s, nextPos, ";"))
      return unique_ptr<Expr>(new Block);
    if (HasToken(s, nextPos, "var")){
      unique_ptr<Definition> def(new Definition);
      unique_ptr<string> varName = getvar(s, nextPos);
      if (varName == 0)
	failparse;
      if (HasToken(s, nextPos, "(")){
	def->isFunction = true;
	if (!HasToken(s, nextPos, ")"))
	  while(true){
	    unique_ptr<string> paramName = getvar(s, nextPos);
	    if (paramName == 0)
	      failparse;
	    def->parameters.push_back(*paramName);
	    if (!HasToken(s, nextPos, ",")){
	      if (HasToken(s, nextPos, ")"))
		break;
	      failparse;
	    }
	  }
      }
      if (!HasToken(s, nextPos, "="))
	failparse;
      if (!HasToken(s, nextPos))
	failparse;
      unique_ptr<Expr> e = parseCommand(s, nextPos);
      // cout << "===" << e->tostring() << " " << (e->kind == ExFunc ? "fun" : "not fun") << endl;
      // unique_ptr<Func> ee = e->FuncMe();
      // cout << ee->name << endl;
      // failparse;
      //if (!HasToken(s, nextPos, ";"))
      //failparse;
      def->name = *varName;
      def->value = move(e);
      return move(def);
    }
    else if (HasToken(s, nextPos, "set")){
      unique_ptr<string> varName = getvar(s, nextPos);
      if (varName == 0)
	failparse;
      if (!HasToken(s, nextPos, "="))
	failparse;
      if (!HasToken(s, nextPos))
	failparse;
      unique_ptr<Expr> e = parse(s, nextPos);
      if (!HasToken(s, nextPos, ";"))
	failparse;
      unique_ptr<Assign> def (new Assign);
      def->name = *varName;
      def->value = move(e);
      return move(def);
    }
    else if (HasToken(s, nextPos, "{")){
      unique_ptr<Expr> c = parseCommands(s, nextPos);
      if (!HasToken(s, nextPos, "}"))
	failparse;
      return c;
    }
    else if (HasToken(s, nextPos, "for")){
      if (!HasToken(s, nextPos, "("))
	failparse;
      unique_ptr<string> varName = getvar(s, nextPos);
      if (varName == 0)
	failparse;
      if (!HasToken(s, nextPos, "in"))
	failparse;
      if (!HasToken(s, nextPos))
	failparse;
      unique_ptr<Expr> e = parse(s, nextPos);
      if (!HasToken(s, nextPos, ")"))
	failparse;
      unique_ptr<Expr> c = parseCommand(s, nextPos);
      unique_ptr<For> fc (new For);
      fc->variable = *varName;
      fc->seq=move(e);
      fc->command=move(c);
      return move(fc);
    }
    else if (HasToken(s, nextPos, "while")){
      if (!HasToken(s, nextPos, "("))
	failparse;
      if (!HasToken(s, nextPos))
	failparse;
      unique_ptr<Expr> cond = parse(s, nextPos);
      if (!HasToken(s, nextPos, ")"))
	failparse;
      unique_ptr<Expr> c = parseCommand(s, nextPos);
      unique_ptr<While> fc (new While);
      fc->cond = move(cond);
      fc->command = move(c);
      return move(fc);
    }
    else if (HasToken(s, nextPos, "if")){
      if (!HasToken(s, nextPos, "("))
	failparse;
      if (!HasToken(s, nextPos))
	failparse;
      unique_ptr<Expr> cond = parse(s, nextPos);
      if (!HasToken(s, nextPos, ")"))
	failparse;
      unique_ptr<Expr> ct = parseCommand(s, nextPos);
      if (!HasToken(s, nextPos, "else"))
	failparse;
      unique_ptr<Expr> ce = parseCommand(s, nextPos);
      unique_ptr<Ifte> fc (new Ifte);
      fc->cond = move(cond);
      fc->commandt = move(ct);
      fc->commande = move(ce);
      return move(fc);
    }
    unique_ptr<Expr> e = parse(s, nextPos);
    if (!HasToken(s, nextPos, ";"))
      failparse;
    // Call* call = new Call;
    // call->f = e;
    return e;
  }

  unique_ptr<Expr> parseCommand(string s){
    int pos = 0;
    return parseCommand(s, pos);
  }

  unique_ptr<Block> parseCommands(string s, int& pos){
    vector<unique_ptr<Expr>> commands;
    while(HasToken(s, pos)){
      int innerPos = pos;
      if(HasToken(s, innerPos, "}"))
	break;
      commands.push_back(parseCommand(s, pos));
    }
    unique_ptr<Block> block (new Block);
    block->commands = move(commands);
    return block;
  }

  unique_ptr<Block> parseCommands(string s){
    int pos = 0;
    return parseCommands(s, pos);
  }

  void pp(unique_ptr<Expr>& command){
    cout << command->tostring() << endl;
  }

  // void pp(unique_ptr<Expr> expr){
  //   switch(expr->kind){
  //   case ExFunc:
  //     pp((unique_ptr<Func>)expr);
  //     return;
  //     break;
  //   }
  //   faileval;
  // }

  // void pp(unique_ptr<Func> func){
  //   if (func->isInfix){
  //     unsigned int size = func->parameters.size();
  //     for(unsigned int i = 0; i < size; i++){
  // 	pp(func->parameters[i]);
  // 	if (i < size - 1)
  // 	  cout << " " << func->name << " ";
  //     }
  //     return;
  //   }
  //   cout << func->name;
  //   if (func->isFunction)
  //     cout << "(";
  //   unsigned int size = func->parameters.size();
  //   for(unsigned int i = 0; i < size; i++)
  //     {
  // 	pp(func->parameters[i]);
  // 	if (i < size - 1)
  // 	  cout << ", ";
  //     }
  //   if (func->isFunction)
  //     cout << ")";
  // }

  ValueAny<double>* doubleValue(Value* p){
      ValueAny<int>* pi = dynamic_cast<ValueAny<int>*>(p);
      if (pi != 0)
	return new ValueAny<double>(pi->value, pi->s);
      return dynamic_cast<ValueAny<double>*>(p);
  }

  ValueAny<int>* intValue(Value* p){
      ValueAny<double>* pd = dynamic_cast<ValueAny<double>*>(p);
      if (pd != 0)
	return new ValueAny<int>(pd->value);
      return dynamic_cast<ValueAny<int>*>(p);
  }

  class FPlus: public FValue {
  public:
    Value* eval(vector<Value*> parameters){
      if (parameters.size() != 2)
	faileval
      Value* p1 = parameters[0];
      Value* p2 = parameters[1];
      ValueAny<int>* p1i = dynamic_cast<ValueAny<int>*>(p1);
      ValueAny<string>* p1s = dynamic_cast<ValueAny<string>*>(p1);
      ValueAny<int>* p2i = dynamic_cast<ValueAny<int>*>(p2);
      ValueAny<string>* p2s = dynamic_cast<ValueAny<string>*>(p2);
      if (p1s != 0 && p2s != 0)
	return new ValueAny<string>(p1s->value + p2s->value, p1s->value + p2s->value);
      else if (p1i != 0 && p2i != 0)
	return new ValueAny<int>(p1i->value + p2i->value, p1i->s + p2i->s);
      ValueAny<double>* p1d = doubleValue(p1);
      if (p1d == 0)
	faileval;
      ValueAny<double>* p2d = doubleValue(p2);
      if (p2d == 0)
	faileval;
      return new ValueAny<double>(p1d->value + p2d->value, "double value");
      

      // if (!(p1->isNum && p2->isNum))
      // 	faileval;
      // if (p1->kind == Int && p2->kind == Int)
      // 	return new Value(p1->intValue + p2->intValue);
      // return new Value(p1->doubleValue + p2->doubleValue);

      // if (p1->kind == p2->kind && p1->kind == String)
      // 	new Value(p1->stringValue + p2->stringValue);
      // else if (!(p1->isNum && p2->isNum))
      // 	faileval;
      // if (p1->kind == Int && p2->kind == Int)
      // 	return new Value(p1->intValue + p2->intValue);
      // return new Value(p1->doubleValue + p2->doubleValue);
    }
    FPlus(){
      name = "+";
    }
  } fplus;

  class FMinus: public FValue {
  public:
    Value* eval(vector<Value*> parameters){
      if (parameters.size() == 1)
  	{
	  Value* p1 = parameters[0];
	  ValueAny<int>* p1i = dynamic_cast<ValueAny<int>*>(p1);
	  if (p1i != 0)
	    return new ValueAny<int>(-p1i->value);
	  ValueAny<double>* p1d = dynamic_cast<ValueAny<double>*>(p1);
	  if (p1d != 0)
	    return new ValueAny<double>(-p1d->value);
	  faileval;
	}
      else if (parameters.size() != 2)
  	faileval;
      Value* p1 = parameters[0];
      Value* p2 = parameters[1];
      ValueAny<int>* p1i = dynamic_cast<ValueAny<int>*>(p1);
      ValueAny<int>* p2i = dynamic_cast<ValueAny<int>*>(p2);
      if (p1i != 0 && p2i != 0)
	return new ValueAny<int>(p1i->value - p2i->value);
      ValueAny<double>* p1d = doubleValue(p1);
      if (p1d == 0)
	faileval;
      ValueAny<double>* p2d = doubleValue(p2);
      if (p2d == 0)
	faileval;
      return new ValueAny<double>(p1d->value - p2d->value);
    }
    FMinus(){
      name = "-";
    }
  } fminus;

#define binop(classname,fname,sname,op) class classname: public FValue { \
public: \
  Value* eval(vector<Value*> parameters){ \
    if (parameters.size() != 2) \
      faileval; \
    Value* p1 = parameters[0]; \
  Value* p2 = parameters[1]; \
  ValueAny<int>* p1i = dynamic_cast<ValueAny<int>*>(p1); \
  ValueAny<int>* p2i = dynamic_cast<ValueAny<int>*>(p2); \
  if (p1i != 0 && p2i != 0) \
    return new ValueAny<int>(p1i->value op p2i->value); \
  ValueAny<double>* p1d = doubleValue(p1); \
  if (p1d == 0) \
    faileval; \
  ValueAny<double>* p2d = doubleValue(p2); \
  if (p2d == 0) \
    faileval; \
  return new ValueAny<double>(p1d->value op p2d->value); \
  } \
  classname(){ \
    name = sname; \
  } \
  } fname

  binop(FTimes,ftimes,"*",*);
  //  binop(FMinus,fminus,"-",- );
  binop(FDiv,fdiv,"/",/);

  // class FTimes: public FValue {
  // public:
  //   Value* eval(vector<Value*> parameters){
  //     if (parameters.size() != 2)
  // 	faileval
  //     Value* p1 = parameters[0];
  //     Value* p2 = parameters[1];
  //     if (p1->kind == p2->kind){
  // 	if (p1->kind == Int)
  // 	  return new Value(p1->intValue * p2->intValue);
  // 	else if (p1->kind == Double)
  // 	  return new Value(p1->doubleValue * p2->doubleValue);
  // 	else faileval;
  //     }
  //     else if (p1->kind == Int && p2->kind == Double)
  // 	return new Value(p1->intValue * p2->doubleValue);
  //     else if (p2->kind == Int && p1->kind == Double)
  // 	return new Value(p2->intValue * p1->doubleValue);
  //     else
  // 	faileval;
  //   }
  //   FTimes(){
  //     name = "*";
  //   }
  // } ftimes;

  class FColor: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(3);
      getint(p1);
      getint(p2);
      getint(p3);
      CImg<unsigned char>* color = new CImg<unsigned char>(CImg<unsigned char>::vector(p1, p2, p3));
      //cout << "COL create " << color->size() << endl;
      return new ValueAny<CImg<unsigned char>*>(color, name);
    }
    FColor(){
      name = "color";
    }
  } fcolor;

  class FSeq: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      int start, step, stop;
      if (parameters.size() == 2){
	check_parameters(2);
	getint(p0);
	getint(p1);
	start = p0;
	stop = p1;
	step = 1;
      } else{
	check_parameters(3);
	getint(p0);
	getint(p1);
	getint(p2);
	start = p0;
	stop = p2;
	step = p1;
      }
      vector<Value*>* v = new vector<Value*>;
      for (int i = start; i <= stop; i += step)
	v->push_back(new ValueAny<int>(i));
      return new ValueAny<vector<Value*>*>(v, vectorinttype);
    }
    FSeq(){
      name = "seq";
    }
  } fseq;

  class FList_Files: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      //int start, step, stop;
      //      if (parameters.size() == 2){
      check_parameters(1);
      getany(string,dirname);

      boost::filesystem::path directory = boost::filesystem::path(dirname);

      vector<string> files;
      
      if( exists( directory ) )
      	{
      	  boost::filesystem::directory_iterator end ;
      	  for( boost::filesystem::directory_iterator iter(directory) ; iter != end ; ++iter )
      	    if ( boost::filesystem::is_directory( *iter ) )
      	      {
      		//cout << iter->native_directory_string() << " (directory)\n" ;
      		//if( recurse_into_subdirs ) show_files(*iter) ;
      	      }
      	    else{
	      files.push_back(iter->path().string());
	    }
      	}
      sort(files.begin(), files.end());
      vector<Value*>* v = new vector<Value*>;
      for(unsigned int i = 0; i < files.size(); i++){
	cout << files[i] << " (file)\n" ;
	v->push_back(new ValueAny<string>(files[i], files[i]));
      }
      return new ValueAny<vector<Value*>*>(v, vectorinttype);
    }
    FList_Files(){
      name = "list_files";
    }
  } flist_files;

  class FPlane: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(10);
      getany(Image::MyMesh*,scene);
      getint(p1);
      getint(p2);
      getint(p3);
      getint(p4);
      getint(p5);
      getint(p6);
      getint(p7);
      getptr(CImg<unsigned char>,fcolor.name,p8);
      getdouble(p9);
      unique_ptr<Image::MyMesh> plane = Image::fmplane3d(p1, p2, p3, p4, p5, p6, p7, *p8, p9);
      Image::append(scene, plane);
      return unit;
    }
    FPlane(){
      name = "plane";
    }
  } fplane;


  class FHPlane: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(10);
      getptr(Image::MyMesh,mymeshtype,scene)
      getint(p1);
      getint(p2);
      getint(p3);
      getint(p4);
      getint(p5);
      getint(p6);
      getint(p7);
      getptr(CImg<unsigned char>,fcolor.name,p8);
      getdouble(p9);
      unique_ptr<Image::MyMesh> plane = Image::fmhplane3d(p1, p2, p3, p4, p5, p6, p7, *p8, p9);
      Image::append(scene, plane);
      return unit;
    }
    FHPlane(){
      name = "hplane";
    }
  } fhplane;

  class FShow: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(1);
      getptr(Image::MyMesh,mymeshtype,scene);
      Image::show(scene);
      return unit;
    }
    FShow(){
      name = "show";
    }
  } fshow;

  class FNew3d: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      if (parameters.size() != 0)
  	faileval;
      return new ValueAny<Image::MyMesh*>(new Image::MyMesh, mymeshtype);
    }
    FNew3d(){
      name = "new3d";
    }
  } fnew3d;

  class FNewmetaball: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      if (parameters.size() != 0)
  	faileval;
      return new ValueAny<Image::Metaballs*>(new Image::Metaballs, mymeshtype);
    }
    FNewmetaball(){
      name = "newmetaball";
    }
  } fnewmetaball;


  class Faddball: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(6);
      getany(Image::Metaballs*,balls);
      getdouble(x);
      getdouble(y);
      getdouble(z);
      getdouble(r);
      getdouble(t);
      balls->add(x, y, z, r, t);
      return unit;
    }
    Faddball(){
      name = "addball";
    }
  } faddball;

  class Fmetaballsmesh: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(4);
      getany(Image::Metaballs*,balls);
      getdouble(target);
      getdouble(r);
      getint(sub);
      Image::MyMesh* mesh = balls->mesh(target, r, sub);
      return new ValueAny<Image::MyMesh*>(mesh, mymeshtype);
    }
    Fmetaballsmesh(){
      name = "metaballsmesh";
    }
  } fmetaballsmesh;


  class FNewimg: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(5);
      getint(p0);
      getint(p1);
      getint(p2);
      getint(p3);
      getint(p4);
      CImg<unsigned char>* img = new CImg<unsigned char>(p0,p1,p2,p3,p4);
      return new ValueAny<CImg<unsigned char>*>(img, cimguchar);
    }
    FNewimg(){
      name = "newimg";
    }
  } fnewimg;

  class Fwait: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(2);
      getptr(CImgDisplay,"CImgDisplay",disp);
      getint(p1);
      disp->wait(p1);
      return unit;
    }
    Fwait(){
      name = "wait";
    }
  } fwait;

  class Fclear: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(1);
      getptr(CImg<unsigned char>,cimguchar,img);
      img->fill(0);
      return unit;
    }
    Fclear(){
      name = "clear";
    }
  } fclear;

  class Fdisplay: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(2);
      getptr(CImg<unsigned char>,cimguchar,img);
      getptr(CImgDisplay,"CImgDisplay",disp);
      disp->display(*img);
      return unit;
    }
    Fdisplay(){
      name = "display";
    }
  } fdisplay;

  class Floadimage: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(1);
      getany(string,p0);
      cout << "READING " << p0 << endl;;
      CImg<unsigned char>* image = new CImg<unsigned char>(CImg<unsigned char>().load(p0.data()));
      cout << "Done " << p0 << " " << image->spectrum() << endl;;
      return new ValueAny<CImg<unsigned char>*>(image, cimguchar);
    }
    Floadimage(){
      name = "loadimage";
    }
  } floadimage;

  class Fcopyimage: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(1);
      getptr(CImg<unsigned char>,cimguchar,img);
      CImg<unsigned char>* image = new CImg<unsigned char>(*img);
      return new ValueAny<CImg<unsigned char>*>(image, cimguchar);
    }
    Fcopyimage(){
      name = "copyimage";
    }
  } fcopyimage;


  class Fresize: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(3);
      getptr(CImg<unsigned char>,cimguchar,img);
      getint(size_x);
      getint(size_y);
      CImg<unsigned char>* image;
      if (size_x == -50 && size_y == -50)
	image = new CImg<unsigned char>(img->get_resize_halfXY());
      else
      if (size_x == -200 && size_y == -200)
	image = new CImg<unsigned char>(img->get_resize_doubleXY());
      else
	image = new CImg<unsigned char>(img->get_resize(size_x, size_y));
      return new ValueAny<CImg<unsigned char>*>(image, cimguchar);
    }
    Fresize(){
      name = "resize";
    }
  } fresize;

  class Fdrawimage: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(6);
      getptr(CImg<unsigned char>,cimguchar,img);
      getptr(CImg<unsigned char>,cimguchar,image);
      getint(p2);
      getint(p3);
      getdouble(p4);
      getint(transparent);
      //cout << "IMG" << img->size() << " " << image->size() << endl;
      img->draw_image_fm(p2, p3, 0, 0, *image, (float)p4, transparent != 0);
      //img->draw_image(p2, p3, 0, 0, *image, (float)p4);
      return unit;
    }
    Fdrawimage(){
      name = "drawimage";
    }
  } fdrawimage;

  class Floadmesh: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(3);
      getany(string,path);
      getptr(CImg<unsigned char>,fcolor.name,col);
      getdouble(opacity);
      cout << "READING " << path << endl;;
      Image::MyMesh* mesh = (new Image::MyMesh())->readOFF(path, *col, opacity);
      cout << "Done " << path << endl;;
      return new ValueAny<Image::MyMesh*>(mesh, mymeshtype);
    }
    Floadmesh(){
      name = "loadmesh";
    }
  } floadmesh;

  class Fdrawmesh: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(9);
      getptr(CImg<unsigned char>,cimguchar,img);
      getptr(Image::MyMesh,mymeshtype,mesh);
      getdouble(x);
      getdouble(y);
      getdouble(z);
      getdouble(p2);
      getdouble(p3);
      getdouble(p4);
      getdouble(focale)
      mesh->drawrotate(img, x, y, z, p2, p3, p4, focale);
      return unit;
    }
    Fdrawmesh(){
      name = "drawmesh";
    }
  } fdrawmesh;

  class Fnewdisplay: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(1);

      getptr(CImg<unsigned char>,cimguchar,img);
      CImgDisplay disp(*img,"Spark");
      // if (false)
      // {
      // 	Image::MyMesh* mesh = (new Image::MyMesh())->readOFF("/home/fmaurel/prog/spark/input/dino2.off");
      // 	mesh->draw(img);
      // 	img->display(disp);
      // }
     
      return new ValueAny<CImgDisplay*>(new CImgDisplay(disp), "CImgDisplay");
    }
    Fnewdisplay(){
      name = "newdisplay";
    }
  } fnewdisplay;

  class Frectangle: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(7);
      getptr(CImg<unsigned char>,cimguchar,img);
      getint(p1);
      getint(p2);
      getint(p3);
      getint(p4);
      getptr(CImg<unsigned char>,fcolor.name,p5);
      getdouble(p6);
      img->draw_rectangle(p1, p2, p3, p4, p5->data(), (float)p6);;
      return unit;
    }
    Frectangle(){
      name = "rectangle";
    }
  } frectangle;

  class Ftext: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      if(parameters.size() == 7){
	check_parameters(7);
	getptr(CImg<unsigned char>,cimguchar,img);
	getint(x);
	getint(y);
	getany(string,s);
	getptr(CImg<unsigned char>,fcolor.name,color);
	getdouble(opacity);
	getint(size);
	img->draw_text(x, y, s.data(), color->data(), 0,(float)opacity, size);
	return unit;
      }
	check_parameters(8);
	getptr(CImg<unsigned char>,cimguchar,img);
	getint(x);
	getint(y);
	getany(string,s);
	getptr(CImg<unsigned char>,fcolor.name,color);
	getptr(CImg<unsigned char>,fcolor.name,bgcolor);
	getdouble(opacity);
	getint(size);
	img->draw_text(x, y, s.data(), color->data(), bgcolor->data(),(float)opacity, size);
	//      img->draw_text(3,3,"Mouse buttons\nto zoom in/out",color,0,0.8f,24)
      return unit;
    }
    Ftext(){
      name = "text";
    }
  } ftext;

  class Fstring: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      if (parameters.size() != 1)
	faileval;
      string s = parameters[0]->tostring();
      return new ValueAny<string>(s, s);
    }
    Fstring(){
      name = "string";
    }
  } fstring;

  void* item_fractal_animation() {
    CImg<unsigned char> img(400,400,1,3,0), noise(3,2,1,3);
    CImgDisplay disp(img,"[#4] - Fractal Animation");
    float zoom = 0;

    for (unsigned int iter = 0; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); ++iter, zoom+=0.2f) {
      img.draw_image((img.width() - noise.width())/2,
		     (img.height() - noise.height())/2,
		     noise.fill(0).noise(255,1)).
	rotate((float)(10*std::sin(iter/25.0)),0.5f*img.width(),0.5f*img.height(),(float)(1.04f + 0.02f*std::sin(zoom/10)),0,0).
	resize(disp.resize(false)).display(disp.wait(25));
      if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(400,400,false).toggle_fullscreen(false);
    }
    return 0;
  }


  class Fsavebmp: public ExpParser::FValue {
  public:
    Value* eval(vector<Value*> parameters){
      check_parameters(2);
      getptr(CImg<unsigned char>,cimguchar,img);
      getint(p1);
      //      getptr(CImg<unsigned char>,fcolor.name,p5);
      string s = boost::lexical_cast<std::string>(p1);
      string s0 = "000000";
      s0.resize(6 - s.size());
      string filename = "/home/fmaurel/prog/spark/output/image" + s0 + s + ".bmp";
      img->save_bmp(filename.data());
      return unit;
    }
    Fsavebmp(){
      name = "savebmp";
    }
  } fsavebmp;



}

string read(string filename){
  ifstream fin(filename.data());
  char result[10000];
  char ch;
  int pos = 0;
  for(; pos < 10000; pos++)
    if (fin.get(ch))
      result[pos] = ch;
    else
      break;
  fin.close();
  string s = result;
  s.resize(pos);
  return s;
}

int main(int argc, char **argv) {
  //  plot2d(argc, argv, "sin(x/8) % cos(2*x)");
  // Read command line argument.
  cimg_usage("Simple plotter of mathematical formulas");

  //string s = "var blue = color(0, 0, 255);var red = color(255, 0, 0);var x = scene; plane(scene, 50, 20, 10, 10, 0, 0, 30, red, 0.9); show(scene);";
  //hplane(scene, 500, 200, 0, 20, 0, blue, 0.5);";
  //string s = "var blue = color(0, 0, 255);var red = color(255, 0, 0); f(x); var x = 3.; var y = 4.; var z = x + y;";
  //  s = "var x = 12.5;";

  ExpParser::Env* env = new ExpParser::Env();
  env->functions.push_back(&ExpParser::fseq);
  env->functions.push_back(&ExpParser::fplus);
  env->functions.push_back(&ExpParser::fminus);
  env->functions.push_back(&ExpParser::ftimes);
  env->functions.push_back(&ExpParser::fdiv);
  env->functions.push_back(&ExpParser::fcolor);
  env->functions.push_back(&ExpParser::fplane);
  env->functions.push_back(&ExpParser::fhplane);
  env->functions.push_back(&ExpParser::fshow);
  env->functions.push_back(&ExpParser::fnew3d);
  env->functions.push_back(&ExpParser::fnewimg);
  env->functions.push_back(&ExpParser::fdisplay);
  env->functions.push_back(&ExpParser::fnewdisplay);
  env->functions.push_back(&ExpParser::frectangle);
  env->functions.push_back(&ExpParser::ftext);
  env->functions.push_back(&ExpParser::fwait);
  env->functions.push_back(&ExpParser::fsavebmp);
  env->functions.push_back(&ExpParser::floadmesh);
  env->functions.push_back(&ExpParser::fdrawmesh);
  env->functions.push_back(&ExpParser::floadimage);
  env->functions.push_back(&ExpParser::fcopyimage);
  env->functions.push_back(&ExpParser::fdrawimage);
  env->functions.push_back(&ExpParser::fresize);
  env->functions.push_back(&ExpParser::fclear);
  env->functions.push_back(&ExpParser::fstring);
  env->functions.push_back(&ExpParser::flist_files);
  env->functions.push_back(&ExpParser::fnewmetaball);
  env->functions.push_back(&ExpParser::faddball);
  env->functions.push_back(&ExpParser::fmetaballsmesh);
  //  env->add("scene", new ExpParser::Value((void*)new Image::fm3d, "fm3d"));
  //  env->names.push_back("scene");
  //env->values.push_back(new ExpParser::Value((void*)"AAA", "fm3d"));
  
  unique_ptr<ExpParser::Block> scenes(new ExpParser::Block);
  string readopt = "-s";
  string evalopt = "-e";

  for (int i = 0; i < (argc - 1) / 2; i++){
    //string filename = "/home/fmaurel/prog/spark/scene.sp";
    string s;
    if (argv[2 * i + 1] == readopt)
      s = read(argv[2 * i + 2]);
    else if (argv[2 * i + 1] == evalopt)
      s = argv[2 * i + 2];
    else{
      cout << "-s <file> | -e <expr>" << endl;
      throw new ExpParser::EvalException();
    }
    
    unique_ptr<ExpParser::Block> c = ExpParser::parseCommands(s);
    for(unsigned int j = 0; j < c->commands.size(); j++)
      scenes->commands.push_back(c->commands[j]->copy());
  }
  here;
  unique_ptr<ExpParser::Expr> e = move(scenes);
  //  pp(e);
  here;
  e->eval(*env);
  here;

  return 0;
}
