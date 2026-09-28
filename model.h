#include "geometry.h"
#include <string>
#include <vector>

class Model {
  std::vector<vec3> verts = {};
  std::vector<int> facets_vrt = {};

public:
  Model(const std::string &fileName);
  int nverts() const;
  int nfacets_vrt() const;
  vec3 vert(const int i) const;
  vec3 vert(const int iface, const int nvert) const;
};
