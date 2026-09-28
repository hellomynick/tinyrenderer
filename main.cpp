#include "tgaimage.h"
#include <array>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer,
          TGAColor color) {
  bool steel = std::abs(bx - ax) < std::abs(by - ay);
  if (steel) {
    std::swap(ax, ay);
    std::swap(bx, by);
  }
  if (ax > bx) {
    std::swap(ax, bx);
    std::swap(ay, by);
  }

  int y = ay;
  int ierror = 0;
  for (int x = ax; x <= bx; x++) {
    if (steel) {
      framebuffer.set(y, x, color);
    } else {
      framebuffer.set(x, y, color);
    }
    ierror += 2 * std::abs(by - ay);
    y += (by > ay ? 1 : -1) * (ierror > bx - ax);
    ierror -= 2 * std::abs(bx - ax) * (ierror > bx - ax);
  }
}

TGAColor random_color() {
  auto b = [] { return static_cast<std::uint8_t>(rand() % 255); };

  return TGAColor{b(), b(), b(), b()};
}

void draw(int width, int height, TGAImage &framebuffer) {
  std::ifstream diabloFile("diablo3_pose.obj");
  std::string line;
  std::vector<std::array<float, 3>> verts;
  std::array<float, 3> points;

  const float aspect = float(width) / height;

  while (std::getline(diabloFile, line)) {
    if (line[0] == 'v' && line[1] == ' ') {
      std::istringstream iss(line);

      char tag;
      float x, y, z;

      iss >> tag >> x >> y >> z;

      points[0] = x;
      points[1] = y;
      points[2] = z;

      verts.push_back(points);
    } else if (line[0] == 'f' && line[1] == ' ') {
      std::istringstream iss(line);

      char tag;
      int f1, f2, f3, t, n;
      char s;
      iss >> tag >> f1 >> s >> t >> s >> n >> f2 >> s >> t >> s >> n >> f3;

      // x(t) = a + t(b - a)
      // t = (x - a) / (b - a)
      // [a;b] = [-1;1]
      // t = (x + 1) / (1 - (-1)) <=> t = (x + 1) / 2
      // px,py = [c;d] = [0;with] & [0;height]
      int ax = std::round(0 + ((verts[f1 - 1][0]) / aspect + 1) * width / 2);
      int ay = std::round(0 + (verts[f1 - 1][1] + 1) * height / 2);
      int bx = std::round(0 + ((verts[f2 - 1][0]) / aspect + 1) * width / 2);
      int by = std::round(0 + (verts[f2 - 1][1] + 1) * height / 2);
      int cx = std::round(0 + ((verts[f3 - 1][0]) / aspect + 1) * width / 2);
      int cy = std::round(0 + (verts[f3 - 1][1] + 1) * height / 2);

      ::line(ax, ay, bx, by, framebuffer, red);
      ::line(bx, by, cx, cy, framebuffer, red);
      ::line(ax, ay, cx, cy, framebuffer, red);
    }
  }
}

int main(int argc, char **argv) {
  constexpr int width = 1920;
  constexpr int height = 1080;
  TGAImage framebuffer(width, height, TGAImage::RGB);

  draw(width, height, framebuffer);

  //   int ax = 7, ay = 3;
  //   int bx = 12, by = 37;
  //   int cx = 62, cy = 53;
  //
  //   line(ax, ay, bx, by, framebuffer, blue);
  //   line(cx, cy, bx, by, framebuffer, green);
  //   line(cx, cy, ax, ay, framebuffer, yellow);
  //   line(ax, ay, cx, cy, framebuffer, red);
  //
  //   framebuffer.set(ax, ay, white);
  //   framebuffer.set(bx, by, white);
  //   framebuffer.set(cx, cy, white);

  //   std::srand(std::time({}));
  //   for (int i = 0; i < (1 << 24); i++) {
  //     int ax = rand() % width, ay = rand() % height;
  //     int bx = rand() % width, by = rand() % height;
  //     line(ax, ay, bx, by, framebuffer, random_color());
  //   }

  framebuffer.write_tga_file("framebuffer.tga");
  return 0;
}
