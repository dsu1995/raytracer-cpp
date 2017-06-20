#include <glm/ext.hpp>
#include <cmath>

#include "A4.hpp"
#include "PhongMaterial.hpp"
#include "polyroots.hpp"

using glm::dvec3;
using glm::dvec4;
using glm::dmat4;

using std::cout;
using std::endl;

const double NEAR_PLANE_DISTANCE = 1;

const double EPS = 0.001;


A4::A4(
    // What to render
    SceneNode* root,

    // Image to write to, set to a given width and height
    Image& image,

    // Viewing parameters
    const glm::vec3& eye,
    const glm::vec3& view,
    const glm::vec3& up,
    double fovy,

    // Lighting parameters
    const glm::vec3& ambient,
    const std::list<Light*>& lights
) : root(root),
    image(image),
    eye(eye),
    view(view),
    up(up),
    fovy(fovy),
    ambient(ambient),
    lights(lights),
    imageWidth(image.width()),
    imageHeight(image.height()),
    rayOrigin(eye, 1)
{
    // Step 1
    const dmat4 T1 = glm::translate(dvec3(imageWidth / -2.0, imageHeight / -2.0, NEAR_PLANE_DISTANCE));
    // Step 2
    const double h = 2 * NEAR_PLANE_DISTANCE * tan(glm::radians(fovy) / 2);
    const dmat4 S2 = glm::scale(dvec3(-h / imageHeight, -h / imageHeight, 1));
    // Step 3
    const dvec3 w = glm::normalize(dvec3(view));
    const dvec3 u = glm::normalize(glm::cross(this->up, w));
    const dvec3 v = glm::cross(w, u);
    const dmat4 R3(
        dvec4(u, 0),
        dvec4(v, 0),
        dvec4(w, 0),
        dvec4(0, 0, 0, 1)
    );
    // Step 4
    const dmat4 T4 = glm::translate(eye);
    MVW = T4 * R3 * S2 * T1;

    initNonHier();

    print();
}

void A4::initNonHier() {
    for (SceneNode* snode: root->children) {
        if (GeometryNode* gnode = dynamic_cast<GeometryNode*>(snode)) {
            Primitive* primitive = gnode->m_primitive;
            if (dynamic_cast<NonhierSphere*>(primitive) != nullptr) {
                nonHierSpheres.push_back(gnode);
            }
            else if (dynamic_cast<NonhierBox*>(primitive) != nullptr) {
                nonHierBoxes.push_back(gnode);
            }
        }
    }
}

glm::dvec3 A4::background(
    uint x,
    uint y,
    const glm::dvec3& rayDirection
) {
//    return {1.0, 1.0, 1.0};
//    return {0.1, 0.1, 0.1};
    return {0, 0, 0};
}

void A4::render() {
    for (uint y = 0; y < imageHeight; y++) {
        for (uint x = 0; x < imageWidth; x++) {
            const dvec4 screenCoordPixel(x, y, 0, 1);
            const dvec4 worldCoordPixel = MVW * screenCoordPixel;
            const dvec4 rayDirection = worldCoordPixel - rayOrigin;

            Hit h = hit(dvec3(rayOrigin), dvec3(rayDirection));

            dvec3 colour;
            if (!h.hasHit) {
                colour = background(x, y, dvec3(rayDirection));
            }
            else {
                PhongMaterial* material = dynamic_cast<PhongMaterial*>(h.node->m_material);
                dvec3 intersectionEps = h.point + glm::normalize(h.normal) * EPS;

                colour = ambient * dvec3(material->m_kd);
                for (Light* light: lights) {
                    if (!between(intersectionEps, light->position)) {
                        colour += rayColour(x, y, dvec3(rayOrigin), dvec3(rayDirection), h, light, 1);
                    }
                }
            }
            image(x, y, 0) = colour[0];
            image(x, y, 1) = colour[1];
            image(x, y, 2) = colour[2];
        }
    }
}

bool A4::between(const glm::dvec3& p1, const glm::dvec3& p2) {
    Hit h = hit(p1, p2 - p1);
    cout << h.hasHit << ' ' << glm::to_string(h.point) << ' ' << h.t << endl;
    return h.hasHit && h.t <= 1;
}

dvec3 A4::rayColour(
    uint x,
    uint y,
    const dvec3& origin,
    const dvec3& ray,
    const A4::Hit& h,
    Light* light,
    uint maxHits
) {
    PhongMaterial* material = dynamic_cast<PhongMaterial*>(h.node->m_material);

    dvec3 k_d = material->m_kd;
    dvec3 l = glm::normalize(dvec3(light->position) - h.point);
    // TODO need to ensure that nothing is between light.position and hit.point
    // if there is, then its a shadow
    dvec3 n = glm::normalize(dvec3(h.normal));
    dvec3 k_s(material->m_ks);
    dvec3 r = glm::normalize(-l + 2 * glm::dot(l, n) * n);
    dvec3 v = glm::normalize(-ray);
    double p = material->m_shininess;
    dvec3 I = light->colour;
    double dist = glm::distance(dvec3(light->position), h.point);

    dvec3 L_in = I / (light->falloff[0] + light->falloff[1] * dist + light->falloff[2] * dist * dist);
    dvec3 L_out = (
        k_d * std::max(0.0, glm::dot(l, n)) + k_s * pow(std::max(0.0, glm::dot(r, v)), p)
    ) * L_in;

    return L_out;
}

struct BoxFace {
    dvec3 point1;
    dvec3 point2;
    dvec3 normal;
};


A4::Hit A4::hit(
    const dvec3& point,
    const dvec3& direction
) {
    double closest = std::numeric_limits<double>::max();
    Hit hit = {false, dvec3(), dvec3(), nullptr, 0};

    for (GeometryNode* node: nonHierBoxes) {
        NonhierBox* box = dynamic_cast<NonhierBox*>(node->m_primitive);
        dvec3 pos = box->m_pos;
        double side = box->m_size;
        std::vector<BoxFace> faces = {
            {pos, pos + dvec3(side, side, 0), {0, 0, -1}},
            {pos, pos + dvec3(side, 0, side), {0, -1, 0}},
            {pos, pos + dvec3(0, side, side), {-1, 0, 0}},
            {pos + dvec3(side, 0, 0), pos + dvec3(side, side, side), {1, 0, 0}},
            {pos + dvec3(0, side, 0), pos + dvec3(side, side, side), {0, 1, 0}},
            {pos + dvec3(0, 0, side), pos + dvec3(side, side, side), {0, 0, 1}}
        };

        for (const BoxFace& face: faces) {
            if (glm::dot(face.normal, direction) >= 0) {
                continue;
            }
            double wecA = glm::dot(point - face.point1, face.normal);
            double wecB = glm::dot(point + direction - face.point1, face.normal);
            double t = wecA / (wecA - wecB);
            if (t < 0) {
                continue;
            }
            dvec3 intersection = point + t * direction;

            if (
                (face.point1.x - EPS <= intersection.x && intersection.x <= face.point2.x + EPS) &&
                (face.point1.y - EPS <= intersection.y && intersection.y <= face.point2.y + EPS) &&
                (face.point1.z - EPS <= intersection.z && intersection.z <= face.point2.z + EPS)
            ) {
                if (t < closest) {
                    closest = t;
                    hit = {true, intersection, face.normal, node, t};
                }
            }
        }
    }

    for (GeometryNode* node: nonHierSpheres) {
        NonhierSphere* sphere = dynamic_cast<NonhierSphere*>(node->m_primitive);

        dvec3 c = sphere->m_pos;
        double r = sphere->m_radius;
        double A = glm::length2(direction);
        double B = glm::dot(direction, point - c) * 2;
        double C = glm::length2(point - c) - r * r;

        double roots[2];
        size_t numRoots = quadraticRoots(A, B, C, roots);

        double t;
        if (numRoots == 0) {
            continue;
        }
        else if (numRoots == 1) {
            t = roots[0];
            if (t < 0) {
                continue;
            }
        }
        else if (numRoots == 2) {
            double t1 = roots[0];
            double t2 = roots[1];
            if (t1 < 0 && t2 < 0) {
                continue;
            }
            else if (t1 >= 0 && t2 >= 0) {
                t = std::min(t1, t2);
            }
            else if (t1 >= 0 && t2 < 0) {
                t = t1;
            }
            else if (t1 < 0 && t2 >= 0) {
                t = t2;
            }
        }

        if (t < closest) {
            closest = t;
            dvec3 intersection = point + t * direction;
            dvec3 normal = intersection - c;
            hit = {true, intersection, normal, node, t};
        }
    }

    return hit;
}


void A4::print() {
    std::cout << "Calling A4_Render(\n" <<
              "\t" << *root <<
              "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
                  "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
              "\t" << "view: " << glm::to_string(view) << std::endl <<
              "\t" << "up:   " << glm::to_string(up) << std::endl <<
              "\t" << "fovy: " << fovy << std::endl <<
              "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
              "\t" << "lights{" << std::endl;

    for (const Light* light : lights) {
        std::cout << "\t\t" <<  *light << std::endl;
    }
    std::cout << "\t}" << std::endl;
    std::cout <<")" << std::endl;

    cout << "Non-hierarchical boxes: " << nonHierBoxes.size() << endl;
    cout << "Non-hierarchical spheres: " << nonHierSpheres.size() << endl;
}