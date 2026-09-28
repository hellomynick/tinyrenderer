#include "model.h"
#include <fstream>
#include <sstream>
#include <string>

Model::Model(const std::string &filename) {
  std::ifstream model_obj(filename);
  std::string line;
  while (std::getline(model_obj, line)) {
    if (line[0] == 'v' && line[1] == ' ') {
      std::istringstream iss(line);

      char tag;
      float x, y, z;

      iss >> tag >> x >> y >> z;

      verts.push_back(vec3{x, y, z});
    } else if (line[0] == 'f' && line[1] == ' ') {
      std::istringstream iss(line);

      char tag;
      int f1, f2, f3, t, n;
      char s;
      iss >> tag >> f1 >> s >> t >> s >> n >> f2 >> s >> t >> s >> n >> f3;

      facets_vrt.push_back(f1 - 1);
      facets_vrt.push_back(f2 - 1);
      facets_vrt.push_back(f3 - 1);
    }
  }
}

int Model::nverts() const { return verts.size(); }

int Model::nfacets_vrt() const { return facets_vrt.size() / 3; }

vec3 Model::vert(const int i) const { return verts[i]; }

vec3 Model::vert(const int nface, const int nvert) const {
  return verts[facets_vrt[nface * 3 + nvert]];
}
