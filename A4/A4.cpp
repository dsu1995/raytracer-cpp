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
//    cout << "imageWidth: " << (imageWidth) << endl;
//    cout << "height: " << (imageHeight) << endl;
//    cout << "asdf: " << glm::to_string(dvec3(imageWidth / -2.0, imageHeight / -2.0, NEAR_PLANE_DISTANCE)) << endl;

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

//    cout << "T1: " << glm::to_string(T1) << endl;
//    cout << "S2: " << glm::to_string(S2) << endl;
//    cout << "R3: " << glm::to_string(R3) << endl;
//    cout << "T4: " << glm::to_string(T4) << endl;


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

//            cout << "MVW: " << glm::to_string(MVW) << endl;

//            cout << "worldCoordPixel: " << glm::to_string(worldCoordPixel) << endl;

            const dvec4 rayDirection = worldCoordPixel - rayOrigin;

//            cout << "rayDirection: " << glm::to_string(rayDirection) << endl;



            dvec3 colour;

            Hit h = hit(dvec3(rayOrigin), dvec3(rayDirection));
            if (!h.hasHit) {
                colour = background(x, y, dvec3(rayDirection));
            }
            else {
                colour = ambient * dvec3(dynamic_cast<PhongMaterial*>(h.node->m_material)->m_kd);
                for (Light* light: lights) {
                    colour += rayColour(x, y, dvec3(rayOrigin), dvec3(rayDirection), h, light, 1);
                }
            }
            image(x, y, 0) = colour[0];
            image(x, y, 1) = colour[1];
            image(x, y, 2) = colour[2];


        }
    }
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

//    cout << "hit! L_out: " << glm::to_string(L_out) << endl;

    return L_out;
}

struct BoxFace {
    dvec3 point1;
    dvec3 point2;
    dvec3 normal;
};

const double EPS = 0.001;

A4::Hit A4::hit(
    const dvec3& point,
    const dvec3& direction
) {
    double closest = std::numeric_limits<double>::max();
    Hit hit = {false, dvec3(), dvec3(), nullptr};

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
            if (glm::dot(face.normal, direction) < 0) {

                double wecA = glm::dot(point - face.point1, face.normal);
                double wecB = glm::dot(point + direction - face.point1, face.normal);
                double t = wecA / (wecA - wecB);
                if (t >= 0) {
                    dvec3 intersection = point + t * direction;

//                    cout << "Point: " << glm::to_string(point) << endl;
//                    cout << "Direction: " << glm::to_string(direction) << endl;
//                    cout << "t: " << t << endl;
//                    cout << "Point 1: " << glm::to_string(face.point1) << endl;
//                    cout << "Point 2: " << glm::to_string(face.point2) << endl;
//                    cout << "Intersection: " << glm::to_string(intersection) << endl << endl;

                    if (
                        (face.point1.x - EPS <= intersection.x && intersection.x <= face.point2.x + EPS) &&
                        (face.point1.y - EPS <= intersection.y && intersection.y <= face.point2.y + EPS) &&
                        (face.point1.z - EPS <= intersection.z && intersection.z <= face.point2.z + EPS)
                    ) {
                        cout << "intersection: " << glm::to_string(intersection) << endl;

                        if (t < closest) {
                            closest = t;
                            hit = {true, intersection, face.normal, node};
                        }
                    }
                }
            }
        }
    }

    for (GeometryNode* node: nonHierSpheres) {
        NonhierSphere* sphere = dynamic_cast<NonhierSphere*>(node->m_primitive);

        dvec3 c = sphere->m_pos;
        double r = sphere->m_radius;
        double A = glm::dot(direction, direction);
        double B = glm::dot(direction, point - c) * 2;
        double C = glm::dot(point - c, point - c) - r * r;

        double roots[2];
        size_t numRoots = quadraticRoots(A, B, C, roots);

        if (numRoots > 0) {
            double closestRoot = roots[0];
            if (numRoots == 2) {
                closestRoot = std::min(roots[0], roots[1]);
            }

            if (closestRoot < closest) {
                closest = closestRoot;
                dvec3 intersection = point + closestRoot * direction;
                dvec3 normal = intersection - c;
                hit = {true, intersection, normal, node};
            }
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