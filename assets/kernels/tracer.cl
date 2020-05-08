uint convertColor(uchar r, uchar g, uchar b) {
  uint result = 0;
  result += r;
  result <<= 8;
  result += g;
  result <<= 8;
  result += b;
  result <<= 8;
  result += 255; // No alpha for now
  return result;
}

typedef struct Sphere {
  float3 position;
  uint3 color;
  float radius;
} Sphere;

kernel void render(global uint *pixels, const uint width, const uint height,
                   global Sphere *spheres, const uint sphereCount) {
  int gid = get_global_id(0);
  uint x = gid % width;
  uint y = gid / width;
  float u = (float)x / (float)width;
  float v = (float)y / (float)height;
  for (int i = 0; i < sphereCount; i++) {

    pixels[gid] = convertColor(spheres[i].position.x, spheres[i].position.y,
                               spheres[i].position.z);
  }
}