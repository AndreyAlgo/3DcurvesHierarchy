//1: CVector
//2: + { CCircle, CEllipse }
//3: save ellipse as 1.bmp
//4: CHelix
//5: shared_ptr
//6: angle2point() -> point()
//7: + int type() 
//8: CFigure *c=fig.get();
//9: list -> vector
//10: populate by random values
//11: sort by STL

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <algorithm>
#include <boost/shared_ptr.hpp>

class CVector {
public:
  double x,y,z;

  CVector(double _x, double _y, double _z=0) {
    x=_x; y=_y; z=_z;
  }
  CVector(const CVector &_v)  {
    x=_v.x; y=_v.y; z=_v.z;
  }
  void out(const char *_s) const {
    printf("%s (%f , %f , %f)\n", _s, x,y,z );   
  }
  CVector& operator=(CVector _v){
    if(this == &_v)
      return *this;
    
    x=_v.x; y=_v.y; z=_v.z;

    return *this;
  }
  CVector operator+(CVector _v) const {
    CVector t= *this;
    t.x += _v.x;
    t.y += _v.y;
    t.z += _v.z;    
    return t;
  }
  CVector operator-(CVector _v) const {
    CVector t= *this;
    t.x -= _v.x;
    t.y -= _v.y;
    t.z -= _v.z;
    return t;
  }
  CVector operator*(double _k) const {
    CVector t= *this;
    t.x *= _k;
    t.y *= _k;
    t.z *= _k;
    return t;
  }
  CVector operator/(double _k) const {
    CVector t= *this;
    t.x /= _k;
    t.y /= _k;
    t.z /= _k;
    return t;
  }
  CVector operator*=(double _k){
    x *= _k;
    y *= _k;
    z *= _k;
    return *this;
  }
};

enum EType { CIRCLE, ELLIPSE, HELIX };

class CFigure 
{
public:
  virtual CVector point(double _angle)=0;
  virtual int type() const =0;
  virtual void out(const char*) const =0;

  //virtual operator double() =0;
  virtual operator double() { return 0; }
  
  CVector derivativ(double _angle){
    const double delta=0.000000001;
    CVector a=point(_angle);
    CVector b=point(_angle+delta);
    CVector res=(b-a)/delta;
    return res;
  }
};

class CCircle : public CFigure
{
protected:
  const CVector center;

public:
  const double radius;
  CCircle(CVector _center, double _radius) : 
    center(_center), radius(_radius)
  {}
  CVector point(double _angle){
    CVector res(radius*cos(_angle),
                radius*sin(_angle));
    return center+res;
  }
  int type() const {
    return CIRCLE; 
  }
  void out(const char *_s) const {
    printf("%s",_s);
    //center.out("center=");
    printf(" R=%f\n",radius);
  }
  operator double(){
    //printf("CCircle operator double\n");
    return radius;
  }
};

class CHelix : public CCircle 
{
public:
  const double step;
  CHelix(CVector _center, double _radius, double _step) : 
    CCircle(_center,_radius), step(_step)
  {}

  CVector point(double _angle) { 
    CVector res=CCircle::point(_angle);
    res.z=_angle*step/2/M_PI;
    return center+res;
  }
  int type() const {
    return HELIX;
  }
  void out(const char *_s) const {
    printf("%s",_s);
    //center.out("center=");
    printf(" R=%f step=%f\n",radius,step);
  }
};

class CEllipse : public CFigure
{
  const CVector center;
  const double a,b;

public:
  CEllipse(CVector _center, double _a, double _b) : 
    center(_center), a(_a), b(_b)
  {}
  CVector point(double _angle) {
    double t, s,c;
    s=sin(_angle); c=cos(_angle);
    t=atan2(a*sin(_angle),b*cos(_angle));
    //printf("t == %f\n",t );
    CVector res(a*cos(t),b*sin(t));    
    return center+res;
  }
  int type() const {
    return ELLIPSE;
  }
  void out(const char *_s) const {
    printf("%s",_s);
    //center.out("center=");
    printf(" A=%f B=%f\n",a,b);
  }
};

typedef boost::shared_ptr<CFigure> PFigure;

#include <vector>
typedef std::vector<PFigure> figure_t;
figure_t figures, circles;

const CVector center(0,0);
int i;

bool cmp_figure( PFigure _a, PFigure _b)
{
  //printf("cmp_figure\n");
  CFigure *f1=_a.get();
  CFigure *f2=_b.get();
  return *f1 < *f2;
}

int main(int argc, char **argv)
{
  //fclose(stdout);

  // populate container in random manner 
  for(i=0;i<1000;i++){
    int _type= (random() *511) % 3;
    //printf("_type=%d\n",_type);
    if(CIRCLE == _type){
      // CIRCLE
      double _radius= /*abs*/( random() % 1000000 )*0.1;
      CCircle *c=new CCircle(center,_radius);
      figures.push_back(PFigure(c));
    }else if(ELLIPSE == _type){
      //ELLIPSE
      CEllipse *e=new CEllipse(center,
        (random() % 1000000 ) *0.1,
        (random() % 1000000 ) *0.1
      );
      //e->out(" e : ");
      figures.push_back(PFigure(e));      
    }else{
      //HELIX
      CHelix *h=new CHelix(center,
        (random() % 1000000 ) *0.1,
        (random() % 1000000 ) *0.1
      );
      figures.push_back(PFigure(h));      
    }
  }

  int count=0;
  printf(" figures : \n");
  figure_t::iterator it = figures.begin();
  while(it != figures.end())
  {
    const char *figure_names[]= { "CIRCLE","ELLIPSE","HELIX" };
    PFigure fig(*it);
    int typ=fig->type();    
    //printf("type [%d] == %d\n",count,typ );
    printf("figure [%d] : %s ",count,figure_names[typ] );
    CFigure *c=fig.get();
    c->out(" : ");
    //printf(" c=%x\n", c);

    fig->point(M_PI/4).out("  point (M_PI/4) : ");
    fig->derivativ(M_PI/4).out("  derivativ (M_PI/4) : ");
    
    if(CIRCLE == typ){
      CCircle *circle=(CCircle*) c;
      //circle->out("    circle : ");
      circles.push_back( fig );
    }
    
    count++;
    it++;
  }

  printf(" sort\n"); //sleep(10);
  sort(circles.begin(),circles.end(), cmp_figure );

  count=0;
  double sum=0;
  it = circles.begin();
  while(it != circles.end())
  {
    PFigure fig(*it);
    CFigure *c=fig.get();
    //printf(" c=%x\n", c);
    
    printf("circle[%d]",count);
    const CCircle *circle=(CCircle*) c;
    circle->out(" : ");
    sum += circle->radius;    
    
    count++;
    it++;
  }
  printf("sum=%f\n",sum);

  return 0;
}
