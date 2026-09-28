#include "tgaimage.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

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

  float y = ay;
  float slope = (by - ay) / static_cast<float>(bx - ax);

  for (int x = ax; x <= bx; x++) {
    int yi = std::round(y);

    if (steel) {
      framebuffer.set(yi, x, color);
    } else {
      framebuffer.set(x, yi, color);
    }
    y += slope;
  }
}

TGAColor random_color() {
  auto b = [] { return static_cast<std::uint8_t>(rand() % 255); };

  return TGAColor{b(), b(), b(), b()};
}

int main(int argc, char **argv) {
  constexpr int width = 64;
  constexpr int height = 64;
  TGAImage framebuffer(width, height, TGAImage::RGB);

  int ax = 7, ay = 3;
  int bx = 12, by = 37;
  int cx = 62, cy = 53;

  line(ax, ay, bx, by, framebuffer, blue);
  line(cx, cy, bx, by, framebuffer, green);
  line(cx, cy, ax, ay, framebuffer, yellow);
  line(ax, ay, cx, cy, framebuffer, red);

  framebuffer.set(ax, ay, white);
  framebuffer.set(bx, by, white);
  framebuffer.set(cx, cy, white);

  framebuffer.write_tga_file("framebuffer.tga");
  return 0;
}
