#include "geometry.h"
#include "model.h"
#include "tgaimage.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>
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

void ModelDraw(int width, int height, TGAImage &framebuffer) {
  Model diablo("diablo3_pose.obj");

  const float aspect = float(width) / height;
  for (int i = 0; i < diablo.nfacets_vrt(); i++) {
    vec3 p[3];

    for (int k = 0; k < 3; k++) {
      p[k] = diablo.vert(i, k);
    }

    for (int k = 0; k < 3; k++) {
      // x(t) = a + t(b - a)
      // t = (x - a) / (b - a)
      // [a;b] = [-1;1]
      // t = (x + 1) / (1 - (-1)) <=> t = (x + 1) / 2
      // px,py = [c;d] = [0;with] & [0;height]

      vec3 a = p[k], b = p[(k + 1) % 3];
      int ax = (a.x / aspect + 1) * width / 2;
      int ay = (a.y + 1) * height / 2;
      int bx = (b.x / aspect + 1) * width / 2;
      int by = (b.y + 1) * height / 2;

      line(ax, ay, bx, by, framebuffer, red);
    }
  }
}
//
// int main(int argc, char **argv) {
//   constexpr int width = 1920;
//   constexpr int height = 1080;
//   TGAImage framebuffer(width, height, TGAImage::RGB);
//
//   ModelDraw(width, height, framebuffer);
//
//   framebuffer.write_tga_file("framebuffer.tga");
//   return 0;
// }

void triangle(int ax, int ay, int bx, int by, int cx, int cy,
              TGAImage &framebuffer, TGAColor color) {

  int minx = std::min({ax, bx, cx}), maxx = std::max({ax, bx, cx});
  int miny = std::min({ay, by, cy}), maxy = std::max({ay, by, cy});

  for (int y = miny; y < maxy; y++) {
    int w1, w2, w3;
    for (int x = minx; x < maxx; x++) {
      w1 = edge(vec2{float(ax), float(ay)}, vec2{float(bx), float(by)},
                vec2{float(x), float(y)});
      w2 = edge(vec2{float(bx), float(by)}, vec2{float(cx), float(cy)},
                vec2{float(x), float(y)});
      w3 = edge(vec2{float(cx), float(cy)}, vec2{float(ax), float(ay)},
                vec2{float(x), float(y)});
      if ((w1 >= 0 && w2 >= 0 && w3 >= 0) || (w1 <= 0 && w2 <= 0 && w3 <= 0)) {
        framebuffer.set(x, y, color);
      }
    }
  }

  line(ax, ay, bx, by, framebuffer, color);
  line(bx, by, cx, cy, framebuffer, color);
  line(cx, cy, ax, ay, framebuffer, color);
}

int main(int argc, char **argv) {
  constexpr int width = 1920;
  constexpr int height = 1080;

  TGAImage framebuffer(width, height, TGAImage::RGB);
  triangle(7, 45, 35, 100, 45, 60, framebuffer, red);
  triangle(120, 35, 90, 5, 45, 110, framebuffer, white);
  triangle(115, 83, 80, 90, 85, 120, framebuffer, green);

  framebuffer.write_tga_file("framebuffer.tga");
  return 0;
}
