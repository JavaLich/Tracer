__kernel void render(__global uint *pixels) {
  int gid = get_global_id(0);
  pixels[gid] = 0xFF00FFFF;
}