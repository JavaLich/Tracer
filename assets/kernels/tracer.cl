#define SPHERE_COUNT 8

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
} __attribute__((aligned(64))) Sphere;

typedef struct Scene {
  Sphere spheres[SPHERE_COUNT];

  float3 cameraPos;
  float screenDistance;
} Scene;

typedef struct Ray {
  float3 origin;
  float3 direction;
  float length;
} Ray;

typedef struct Light {
  float3 color;
  float3 position;
} Light;

bool raySphereIntersection(Ray *ray, Sphere sphere) {
  float3 originToSphere = sphere.position - ray->origin;
  float projection = dot(originToSphere, ray->direction);
  float3 distanceVector = originToSphere - ray->direction * projection;
  float distanceSq = dot(distanceVector, distanceVector);
  float radiusSq = sphere.radius * sphere.radius;

  if (distanceSq > radiusSq) {
    return false;
  }
  float newLength = projection - sqrt(radiusSq - distanceSq);
  if (newLength < ray->length && newLength > 0.0) {
    ray->length = newLength;
    return true;
  }
  return false;
}

kernel void render(global uint *pixels, const uint width, const uint height,
                   global Sphere *spheres, const uint sphereCount,
                   global float3 *rot, global Light *lights,
                   const uint light_count, global Scene *scene) {
  float aspect_ratio = (float)width / (float)height;
  int gid = get_global_id(0);
  uint x = gid % width;
  uint y = gid / width;
  float u = (float)x / (float)width;
  float v = (float)y / (float)height;
  float3 viewDir = (float3)(0.0, 0.0, 1.0);
  viewDir = (float3)(
      rot[0].x * viewDir.x + rot[0].y * viewDir.y + rot[0].z * viewDir.z,
      rot[1].x * viewDir.x + rot[1].y * viewDir.y + rot[1].z * viewDir.z,
      rot[2].x * viewDir.x + rot[2].y * viewDir.y + rot[2].z * viewDir.z);
  float3 screenCenter = scene[0].cameraPos + viewDir * scene[0].screenDistance;
  float3 offset = (float3)(-1.0, 1.0 / aspect_ratio, 0.0);
  float3 offset0 = (float3)(1.0, 1.0 / aspect_ratio, 0.0);
  float3 offset1 = (float3)(-1.0, -1.0 / aspect_ratio, 0.0);
  offset =
      (float3)(rot[0].x * offset.x + rot[0].y * offset.y + rot[0].z * offset.z,
               rot[1].x * offset.x + rot[1].y * offset.y + rot[1].z * offset.z,
               rot[2].x * offset.x + rot[2].y * offset.y + rot[2].z * offset.z);
  offset0 = (float3)(
      rot[0].x * offset0.x + rot[0].y * offset0.y + rot[0].z * offset0.z,
      rot[1].x * offset0.x + rot[1].y * offset0.y + rot[1].z * offset0.z,
      rot[2].x * offset0.x + rot[2].y * offset0.y + rot[2].z * offset0.z);
  offset1 = (float3)(
      rot[0].x * offset1.x + rot[0].y * offset1.y + rot[0].z * offset1.z,
      rot[1].x * offset1.x + rot[1].y * offset1.y + rot[1].z * offset1.z,
      rot[2].x * offset1.x + rot[2].y * offset1.y + rot[2].z * offset1.z);
  float3 p0 = screenCenter + offset;
  float3 p1 = screenCenter + offset0;
  float3 p2 = screenCenter + offset1;

  float3 pointOnScreen = p0 + (p1 - p0) * u + (p2 - p0) * v;
  float3 rayDirection = pointOnScreen - scene[0].cameraPos;

  Ray ray = {scene[0].cameraPos, normalize(rayDirection), 10000000.0};
  int hitIndex = -1;
  for (int i = 0; i < 8; i++) {
    if (raySphereIntersection(&ray, scene->spheres[i])) {
      hitIndex = i;
    }
  }
  uint3 finalColor = (uint3)(0, 0, 0);
  for (int i = 0; i < light_count; i++) {

    if (hitIndex != -1) {
      float3 hitPoint = ray.origin + ray.direction * ray.length;
      float3 normal = normalize(hitPoint - scene->spheres[hitIndex].position);
      float3 lightRayDirection = normalize(lights[i].position - hitPoint);
      float len = length(lights[i].position - hitPoint);
      Ray lightRay = {hitPoint, lightRayDirection, len};
      bool occluded = false;
      for (int i = 0; i < 8; i++) {
        if (raySphereIntersection(&lightRay, scene->spheres[i])) {
          occluded = true;
          break;
        }
      }
      if (!occluded) {

        float d = dot(normal, lightRayDirection);
        if (d < 0.0)
          d = 0.0;
        float3 color = lights[i].color * d;
        finalColor += (uint3)(color.x * scene->spheres[hitIndex].color.x,
                              color.y * scene->spheres[hitIndex].color.y,
                              color.z * scene->spheres[hitIndex].color.z);
      }
    }
  }
  finalColor = clamp(finalColor, (uint3)(0, 0, 0), (uint3)(255, 255, 255));
  pixels[gid] = convertColor(255, 255, 255);
  // finalColor.x, finalColor.y, finalColor.z
}