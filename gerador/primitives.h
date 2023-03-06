#include <vector>
#include "../aux/ponto.h"

using namespace std;

vector<Ponto> plane(float dim,float div);
vector<Ponto> box(float dim,float div);
vector <Ponto> cone(float raio, float altura, int slices, int stacks);
vector<Ponto> sphere(float raio, int slices, int stacks);

