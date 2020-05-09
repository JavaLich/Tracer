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

typedef struct Scene {
  float3 cameraPos;
  float3 viewDir;
  float screenDistance;
} Scene;

typedef struct Ray {
  float3 origin;
  float3 direction;
  float length;
} Ray;

bool raySphereIntersection(Ray ray, Sphere sphere) {
  float3 originToSphere = sphere.position - ray.origin;
  float projection = dot(originToSphere, ray.direction);
  float3 distanceVector = originToSphere - ray.direction * projection;
  float distanceSq = dot(distanceVector, distanceVector);
  float radiusSq = sphere.radius * sphere.radius;

  if (distanceSq > radiusSq) {
    return false;
  }
  float newLength = projection - sqrt(radiusSq - distanceSq);
  if (newLength < ray.length && newLength > 0.0) {
    ray.length = newLength;
    return true;
  }
  return false;
}

kernel void render(global uint *pixels, const uint width, const uint height,
                   global Sphere *spheres, const uint sphereCount,
                   constant Scene *scene) {
  int gid = get_global_id(0);
  uint x = gid % width;
  uint y = gid / width;
  float u = (float)x / (float)width;
  float v = (float)y / (float)height;

  float3 screenCenter =
      scene[0].cameraPos + scene[0].viewDir * scene[0].screenDistance;
  float3 p0 = screenCenter + (float3)(-1.0, (float)height / (float)width, 0.0);
  float3 p1 = screenCenter + (float3)(1.0, (float)height / (float)width, 0.0);
  float3 p2 = screenCenter + (float3)(-1.0, -(float)height / (float)width, 0.0);

  float3 pointOnScreen = p0 + (p1 - p0) * u + (p2 - p0) * v;
  float3 rayDirection = pointOnScreen - scene[0].cameraPos;

  Ray ray = {scene[0].cameraPos, normalize(rayDirection), 10000000.0};

  for (int i = 0; i < sphereCount; i++) {
    if (raySphereIntersection(ray, spheres[i])) {
      pixels[gid] = convertColor(spheres[i].color.x, spheres[i].color.y,
                                 spheres[i].color.z);
    }
  }
}