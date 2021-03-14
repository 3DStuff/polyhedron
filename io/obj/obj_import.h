#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"

#include "../../tinyobjloader/tiny_obj_loader.h"

#pragma GCC diagnostic pop

#include "../../mesh/polyhedron.h"
#include "../../glm_ext/glm_extensions.h"

#include <cassert>
#include <tuple>
#include <algorithm>
#include <limits>
#include <cmath>
#include <utility>
#include <limits>
#include <vector>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <set>
#include <map>
#include <tuple>
#include <sstream>


namespace mesh {
    template<class T>
    class polyhedron;
}

namespace obj {
    class format {
    private:
        tinyobj::attrib_t _attrib;
        std::vector<tinyobj::shape_t> _shapes;
        std::vector<tinyobj::material_t> _materials;
        std::string _warn;
        std::string _err;
        bool _loadSuccess = false;

    public:
        format() = default;
        format(const std::string &file) {
            _loadSuccess = tinyobj::LoadObj(&_attrib, &_shapes, &_materials, &_warn, &_err, file.c_str());
        }

        //! merge all the crap together
        mesh::polyhedron<float> to_polyhedron() {
            mesh::polyhedron<float> poly;

            uint32_t face_counter = 0;
            for(auto &m : _shapes) {
                auto &vertex_arr = poly._vertices;
                auto &index_arr = poly._indices;

                // build index buffer
                for(uint32_t f = 0; f < m.mesh.num_face_vertices.size(); f++) {
                    assert(m.mesh.num_face_vertices[f] == 3);

                    const uint32_t id1 = m.mesh.indices[3*f+0].vertex_index;
                    const uint32_t id2 = m.mesh.indices[3*f+1].vertex_index;
                    const uint32_t id3 = m.mesh.indices[3*f+2].vertex_index;

                    vertex_arr.push_back(glm::make_vec3(&_attrib.vertices[3*id1]));
                    vertex_arr.push_back(glm::make_vec3(&_attrib.vertices[3*id2]));
                    vertex_arr.push_back(glm::make_vec3(&_attrib.vertices[3*id3]));

                    index_arr.add(mesh::face(face_counter+0, face_counter+1, face_counter+2));
                    face_counter += 3;
                }
            }
            return poly;
        }

        static void save(mesh::polyhedron<float> &poly, const std::string &filename) {
            std::ofstream f = std::ofstream(filename);
            f << "#VoxelMagick SIMPLIFIED OBJ EXPORT\n";
            for(auto &v : poly._vertices) {
                f << "v " << v.x << " " << v.y << " " << v.z << std::endl;
            }

            for(auto &face : poly._indices._buffer) {
                const uint32_t id1 = face._ids[0]+1;
                const uint32_t id2 = face._ids[1]+1;
                const uint32_t id3 = face._ids[2]+1;
                f << "f " << id1 << " " << id2 << " " << id3 << std::endl;
            }
            f.close();
        }

        static void save(const std::vector<mesh::polyhedron<float>> &polys, const std::string &filename) {
            // first buffer into a string
            std::stringstream ss;
            ss << "#VoxelMagick SIMPLIFIED OBJ EXPORT\n";

            size_t vid_offs = 0;
            for(auto &poly : polys) {
                ss << "o " << poly._name << std::endl;

                for(auto &v : poly._vertices) {
                    ss << "v " << v.x << " " << v.y << " " << v.z << std::endl;
                }

                for(auto &face : poly._indices._buffer) {
                    const uint32_t id1 = face._ids[0]+1;
                    const uint32_t id2 = face._ids[1]+1;
                    const uint32_t id3 = face._ids[2]+1;
                    ss << "f " << id1+vid_offs << " " << id2+vid_offs << " " << id3+vid_offs << std::endl;
                }

                vid_offs += poly._vertices.size();
            }

            // write to file
            std::ofstream f = std::ofstream(filename);
            f << ss.str();
            f.close();
        }
    };
};
