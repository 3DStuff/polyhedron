#pragma once

#include "../../glm_ext/glm_extensions.h"
#include "types.h"

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


namespace mesh {
    template<class T>
    class polyhedron;

    template<class T>
    struct bbox;
}

namespace stl {
    class format {
        // small check to guarantee sanity
        static_assert(sizeof(face) == sizeof(glm::vec3) * sizeof(float) + sizeof(uint16_t), "size mismatch: face not compatible with stl format");
       
        uint8_t _header[80] = { 0 };
        std::vector<face> _faces;
       
        public:
            format() = default;
            format(const std::string &file);

            static mesh::polyhedron<float> to_polyhedron(const std::vector<face> &faces);
            static void save(const std::vector<face> &, const std::string &filename);
            
            static std::ofstream open(const std::string &file);
            static void close(std::ofstream &f);
            template<typename T>
            static void append(std::ofstream &f, const T & data) {
                f.write((char*)(&data), sizeof(T));
            }
            template<typename T>
            static void append(std::ofstream &f, const std::vector<T> & data) {
                assert(data.size() > 0);
                f.write((char*)(&data[0]), sizeof(T));
            }
            template<typename T>
            static void append(std::ofstream &f, const T & data, const size_t bytes) {
                if(bytes == 0) return;
                f.write((char*)(&data), bytes);
            }
            template<typename T>
            static void append(std::ofstream &f, const std::vector<T> & data, const size_t bytes) {
                if(bytes == 0) return;
                f.write((char*)(&data[0]), bytes);
            }
            
            bool load(const std::string &filename);
            const std::vector<face> &faces() const;
                   
            //! calculates a bounding box
            static mesh::bbox<float> estimate_bbox(const std::vector<face> &);
            //! this functions centers the mesh automatically around around {0,0,0}
            static std::vector<face> remove_offset(const mesh::bbox<float> &, const std::vector<face> &);
            //! this functions centers the mesh automatically around around {0,0,0}
            //! and scales it down to 0 <= {x,y,z} <= 1
            static std::vector<face> normalized(const mesh::bbox<float> &, const std::vector<face> &, const glm::vec3 &transl = glm::vec3(0));
           
            // operator overload to access face
            face& operator[](std::size_t idx) { return _faces[idx]; }
            const face& operator[](std::size_t idx) const { return _faces[idx]; }
    };
};
