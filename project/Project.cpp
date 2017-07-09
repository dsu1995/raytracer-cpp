#include <glm/ext.hpp>
#include <cmath>

#include "Project.hpp"
#include "PhongMaterial.hpp"
#include "polyroots.hpp"

using glm::dvec3;
using glm::dvec4;
using glm::dmat3;
using glm::dmat4;

using std::cout;
using std::endl;

const double NEAR_PLANE_DISTANCE = 1;

const double EPS = 0.0001;

const bool SHOW_BOUNDING_BOXES = false;

const bool SUPERSAMPLE_ON = true;

Project::Project(
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

    print();
}

glm::dvec3 Project::background(
    double x,
    double y,
    const glm::dvec3& rayDirection
) {
    x /= imageWidth;
    y /= imageHeight;
    return {x, x * y, y};
}

const double INCREMENT = 0.01;

void Project::render() {
    size_t total = imageWidth * imageHeight;
    size_t complete = 0;
    double nextGoal = INCREMENT;

    for (uint y = 0; y < imageHeight; y++) {
        for (uint x = 0; x < imageWidth; x++, complete++) {
            dvec3 colour(0, 0, 0);

            if (SUPERSAMPLE_ON) {
                // super sample 9 times per pixel
                double delta = 1.0 / 3;
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        colour += renderPixel(x + i * delta, y + j * delta);
                    }
                }
                colour /= 9;
            }
            else {
                colour = renderPixel(x, y);
            }

            image(x, y, 0) = colour[0];
            image(x, y, 1) = colour[1];
            image(x, y, 2) = colour[2];
        }

        if (complete > total * nextGoal) {
            nextGoal += INCREMENT;
            cout << "Pixels rendered: " << complete << '/' << total << '\r';
            cout.flush();
        }
    }
    cout << "Pixels rendered: " << complete << '/' << total << endl;
}

dvec3 Project::renderPixel(double x, double y) {
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
                colour += rayColour(dvec3(rayOrigin), dvec3(rayDirection), h, light, 1);
            }
        }
    }

    return colour;
}

const Project::Hit& Project::minHit(
    const dvec3 origin,
    const Project::Hit& h1,
    const Project::Hit& h2
) const {
    if (!h1.hasHit && !h2.hasHit) {
        return h1;
    }
    else if (h1.hasHit && !h2.hasHit) {
        return h1;
    }
    else if (!h1.hasHit && h2.hasHit) {
        return h2;
    }
    else {
        if (glm::distance(origin, h1.point) < glm::distance(origin, h2.point)) {
            return h1;
        }
        else {
            return h2;
        }
    }
}

bool Project::between(const glm::dvec3& p1, const glm::dvec3& p2) {
    Hit h = hit(p1, p2 - p1);
    return h.hasHit && h.t <= 1;
}

dvec3 Project::rayColour(
    const dvec3& origin,
    const dvec3& direction,
    const Project::Hit& hit,
    Light* light,
    uint maxHits
) {
    PhongMaterial* material = dynamic_cast<PhongMaterial*>(hit.node->m_material);

    dvec3 k_d = material->m_kd;
    dvec3 l = glm::normalize(dvec3(light->position) - hit.point);
    dvec3 n = glm::normalize(dvec3(hit.normal));
    dvec3 k_s(material->m_ks);
    dvec3 r = glm::normalize(-l + 2 * glm::dot(l, n) * n);
    dvec3 v = glm::normalize(-direction);
    double p = material->m_shininess;
    dvec3 I = light->colour;
    double dist = glm::distance(dvec3(light->position), hit.point);

    dvec3 L_in = I / (light->falloff[0] + light->falloff[1] * dist + light->falloff[2] * dist * dist);
    dvec3 L_out = (
        k_d * std::max(0.0, glm::dot(l, n)) + k_s * pow(std::max(0.0, glm::dot(r, v)), p)
    ) * L_in;

    return L_out;
}


Project::Hit Project::rayTriangleIntersect(
    const glm::dvec3& p0,
    const glm::dvec3& p1,
    const glm::dvec3& p2,
    const glm::dvec3& origin,
    const glm::dvec3& direction,
    GeometryNode* node
) {
    dvec3 R = origin - p0;

    dmat3 M(
        p1 - p0,
        p2 - p0,
        -direction
    );

    double D = glm::determinant(M);

    dmat3 M1(M);
    M1[0] = R;
    double D1 = glm::determinant(M1);

    dmat3 M2(M);
    M2[1] = R;
    double D2 = glm::determinant(M2);

    dmat3 M3(M);
    M3[2] = R;
    double D3 = glm::determinant(M3);

    double beta = D1 / D;
    double gamma = D2 / D;
    double t = D3 / D;

    if (beta >= 0 && gamma >= 0 && beta + gamma <= 1 && t >= 0) {
        dvec3 intersection = origin + t * direction;
        dvec3 normal = glm::cross(p1 - p0, p2 - p1); // TODO check normal direction
        return {true, intersection, normal, node, t};
    }
    else {
        return {false, dvec3(), dvec3(), nullptr, 0.0};
    }
}

Project::Hit Project::hierHit(
    SceneNode* node,
    const dvec3& origin,
    const dvec3& direction
) {
    dmat4 inv = node->get_inverse();
    dvec3 newOrigin(inv * dvec4(origin, 1));
    dvec3 newDirection(inv * dvec4(direction, 0));

    Hit closest = {false, dvec3(), dvec3(), nullptr, 0};

    GeometryNode * const gnode = dynamic_cast<GeometryNode*>(node);
    if (gnode != nullptr) {
        if (Mesh* mesh = dynamic_cast<Mesh*>(gnode->m_primitive)) {
            if (SHOW_BOUNDING_BOXES) {
                NonhierBox boundingBox = mesh->boundingBox();
                Hit hit = nonHierBoxIntersect(&boundingBox, newOrigin, newDirection, gnode);

                closest = minHit(newOrigin, closest, hit);
            }
            else {
                NonhierBox boundingBox = mesh->boundingBox();
                Hit boundingBoxHit = nonHierBoxIntersect(&boundingBox, newOrigin, newDirection, gnode);

                if (boundingBoxHit.hasHit) {
                    Hit hit = meshIntersect(mesh, newOrigin, newDirection, gnode);

                    closest = minHit(newOrigin, closest, hit);
                }
            }
        }
        else if (NonhierBox* box = dynamic_cast<NonhierBox*>(gnode->m_primitive)) {
            Hit hit = nonHierBoxIntersect(box, newOrigin, newDirection, gnode);

            closest = minHit(newOrigin, closest, hit);
        }
        else if (NonhierSphere* nhsphere = dynamic_cast<NonhierSphere*>(gnode->m_primitive)) {
            Hit hit = nonHierSphereIntersect(nhsphere, newOrigin, newDirection, gnode);

            closest = minHit(newOrigin, closest, hit);
        }
    }

    for (SceneNode* child: node->children) {
        Hit hit = hierHit(child, newOrigin, newDirection);

        closest = minHit(newOrigin, closest, hit);
    }

    if (closest.hasHit) {
        closest.point = dvec3(node->get_transform() * glm::vec4(closest.point, 1));

        dmat3 normalTransform(node->get_inverse());
        normalTransform = glm::transpose(normalTransform);
        closest.normal = normalTransform * closest.normal;
    }

    return closest;
}


struct BoxFace {
    dvec3 point1;
    dvec3 point2;
    dvec3 normal;
};


Project::Hit Project::hit(
    const dvec3& origin,
    const dvec3& direction
) {
    return hierHit(root, origin, direction);
}

Project::Hit Project::nonHierSphereIntersect(
    NonhierSphere* sphere,
    const glm::dvec3 origin,
    const glm::dvec3 direction,
    GeometryNode* node
) {
    Hit closest = {false, dvec3(), dvec3(), nullptr, 0};

    dvec3 c = sphere->m_pos;
    double r = sphere->m_radius;
    double A = glm::length2(direction);
    double B = glm::dot(direction, origin - c) * 2;
    double C = glm::length2(origin - c) - r * r;

    double roots[2];
    size_t numRoots = quadraticRoots(A, B, C, roots);

    double t;
    if (numRoots == 0) {
        return closest;
    }
    else if (numRoots == 1) {
        t = roots[0];
        if (t < 0) {
            return closest;
        }
    }
    else if (numRoots == 2) {
        double t1 = roots[0];
        double t2 = roots[1];
        if (t1 < 0 && t2 < 0) {
            return closest;
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
        else {
            assert(false);
        }
    }
    else {
        assert(false);
    }

    dvec3 intersection = origin + t * direction;
    dvec3 normal = intersection - c;
    return {true, intersection, normal, node, t};
}

Project::Hit
Project::nonHierBoxIntersect(
    NonhierBox* box,
    const glm::dvec3 origin,
    const glm::dvec3 direction,
    GeometryNode* node
) {
    Hit closest = {false, dvec3(), dvec3(), nullptr, 0};

    dvec3 pos = box->m_pos;
    dvec3 dims = box->dims;
    std::vector<BoxFace> faces = {
        {pos, pos + dvec3(dims.x, dims.y, 0), {0, 0, -1}},
        {pos, pos + dvec3(dims.x, 0, dims.z), {0, -1, 0}},
        {pos, pos + dvec3(0, dims.y, dims.z), {-1, 0, 0}},
        {pos + dvec3(dims.x, 0, 0), pos + dims, {1, 0, 0}},
        {pos + dvec3(0, dims.y, 0), pos + dims, {0, 1, 0}},
        {pos + dvec3(0, 0, dims.z), pos + dims, {0, 0, 1}}
    };

    for (const BoxFace& face: faces) {
        // Backface culling breaks when the ray starts inside the cube
//        if (glm::dot(face.normal, direction) >= 0) {
//            continue;
//        }
        double wecA = glm::dot(origin - face.point1, face.normal);
        double wecB = glm::dot(origin + direction - face.point1, face.normal);
        double t = wecA / (wecA - wecB);
        if (t < 0) {
            continue;
        }
        dvec3 intersection = origin + t * direction;

        if (
            (face.point1.x - EPS <= intersection.x && intersection.x <= face.point2.x + EPS) &&
            (face.point1.y - EPS <= intersection.y && intersection.y <= face.point2.y + EPS) &&
            (face.point1.z - EPS <= intersection.z && intersection.z <= face.point2.z + EPS)
            ) {
            Hit hit = {true, intersection, face.normal, node, t};

            closest = minHit(origin, closest, hit);
        }
    }

    return closest;
}

Project::Hit Project::meshIntersect(
    Mesh* mesh,
    const glm::dvec3 origin,
    const glm::dvec3 direction,
    GeometryNode* node
) {
    Hit closest = {false, dvec3(), dvec3(), nullptr, 0};

    const std::vector<glm::vec3>& vertices = mesh->vertices();
    for (const Triangle& triangle: mesh->faces()) {
        Hit triangleHit = rayTriangleIntersect(
            vertices.at(triangle.v1),
            vertices.at(triangle.v2),
            vertices.at(triangle.v3),
            origin,
            direction,
            node
        );

        closest = minHit(origin, closest, triangleHit);
    }

    return closest;
}


void Project::print() {
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
}