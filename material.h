#ifndef NE_MATERIAL_H
#define NE_MATERIAL_H

#include "ray.h"
#include "vec.h"
#include "entity.h"
#include "color.h"
#include "shader.h"

namespace ne {

struct Hit;

class Material {
public:
    Shader shader;
    Color albedo;

    virtual bool scatter(
        const Ray &r_in, const Hit &hit, Color &attenuation, Ray &r_out
    ) const = 0;

    virtual Color emitted(float u, float v, const Vec3 &p) const {
        return Color::Black;
    }
};

class Diffuse : public Material {
public:
    Diffuse(const Shader &s, const Color &a) {
        shader = s;
        albedo = a;
    }

    virtual bool scatter(
        const Ray &r_in, const Hit &hit, Color &attenuation, Ray &r_out
    ) const;
};

class Metal : public Material {
public:
    float roughness;

    Metal(const Shader &s, const Color &a, float roughness)
        : roughness(roughness)
    {
        shader = s;
        albedo = a;
    }

    virtual bool scatter(
        const Ray &r_in, const Hit &hit, Color &attenuation, Ray &r_out
    ) const;
};

class Dielectric : public Material {
public:
    Color albedo;
    float ri;

    Dielectric(const Color &albedo, float refraction_index)
        : albedo(albedo), ri(refraction_index) {}

    Dielectric(float refraction_index)
        : albedo(Color::White), ri(refraction_index) {}

    virtual bool scatter(
        const Ray &r_in, const Hit &hit, Color &attenuation, Ray &r_out
    ) const;
};

class Light : public Material {
public:
    Color emit;

    Light(const Color &emit) : emit(emit) {}

    virtual Color emitted(float u, float v, const Vec3 &p) const {
        return emit;
    }

    virtual bool scatter(
        const Ray &r_in, const Hit &hit, Color &attenuation, Ray &r_out
    ) const {
        return false;
    }
};

} // ne

#endif // NE_MATERIAL_H
