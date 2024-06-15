#include <simplecpp>
#include <fstream>
#include <assert.h>
//V3.h
class V3{
  double x, y, z;
 public:
  V3(double x=0, double y=0, double z=0);
  V3 operator+(V3 const &w) const;
  V3 operator-(V3 const &w) const;
  V3 operator*(double t);
  double getx();   // accessor functions
  double gety();
  double getz();
  friend ostream & operator<<(ostream & ost, V3 v);
  double length();
};

ostream & operator<<(ostream & ost, V3 v);
//star.h
class Star {
private:
  Circle image;
  double mass;
  V3 r,v;  // position, velocity
public:
  Star(){};
  void init(double m, V3 position, V3 velocity);
  void vStep(double dT, V3 f);
  void rStep(double dT);
  void print();
  V3 forceOf(Star &s);
};

//V3.cpp
V3::V3(double x1, double y1, double z1){
  x = x1;
  y = y1;
  z = z1;
}

V3 V3::operator+(V3 const &w)const { return V3(x+w.x, y+w.y, z+w.z); }
V3 V3::operator-(V3 const &w)const { return V3(x-w.x, y-w.y, z-w.z); }

V3 V3::operator*(double t){ return V3(x*t, y*t, z*t); }

double V3::length(){ return sqrt(x*x+y*y+z*z); }

double V3::getx(){return x;}
double V3::gety(){return y;}
double V3::getz(){return z;}

ostream & operator<<(ostream & ost, V3 v){
  ost << "(" << v.x << ", "<< v.y << ", "<< v.z << ")";
  return ost;
}
//star.cpp
void Star::init(double m, V3 r1, V3 v1){
  mass = m;
  r = r1;
  v=v1;
  image.reset(r.getx(),r.gety(),15);
  image.setColor(COLOR("red"));
  image.setFill(true);
  image.penDown();
}

void Star::vStep(double dT, V3 f){
    v = v + f*(dT/mass);
}

void Star::rStep(double dT){
    V3 d = v*dT;
    image.move(d.getx(),d.gety());
    r = r + d;
}

void Star::print(){
    cout << mass << " " << image.getOrigin().getX() << " "
	 << image.getOrigin().getY() << " ";
    cout << v << endl;
}

V3 Star::forceOf(Star &s){
  V3 R = s.r - r;
  return R * (mass * s.mass / pow(R.length(),3));
}
//main.cpp
void read_star_data(Star stars[], int n){
  float mass, vx, vy, vz,  x,y,z;
  for(int i=0; i<n; i++){
    cin >> mass >> x >> y >> z >> vx >> vy >> vz;
    stars[i].init(mass, V3(x,y,z), V3(vx,vy,vz));
  }
  assert(cin);  // quick check that input was valid
}

void calculate_net_force(int n, Star stars[], V3 forces[]){
  for(int i=0; i<n; i++) forces[i]=V3(0,0,0);

  for(int i=0; i<n-1; i++){
    for(int j=i+1; j<n; j++){
      V3 f = stars[i].forceOf(stars[j]);
      forces[i] = forces[i] + f;
      forces[j] = forces[j] - f;
    }
  }
}

int main(){
  initCanvas("N body problem",800,800);

  double T, delta; cin >> T >> delta;
  int n; cin >> n;
  Star stars[n];
  read_star_data(stars, n);

  V3 forces[n];

  calculate_net_force(n,stars,forces);
  for(int i=0; i<n; i++) stars[i].vStep(delta/2, forces[i]);

  for(float t=0; t<T; t+=delta){
    for(int i=0; i<n; i++) stars[i].rStep(delta);
    calculate_net_force(n,stars,forces);
    for(int i=0; i<n; i++) stars[i].vStep(delta, forces[i]);
  }
  getClick();
}
