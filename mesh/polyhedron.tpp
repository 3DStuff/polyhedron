#include "../timer.h"

namespace mesh {
    template<typename base_t>
    void polyhedron<base_t>::scale(const base_t dim) {
        benchmark::timer tmp("scale()");
        const bbox bbox = bounding_box();
        const glm::vec<3, base_t> cur_size = bbox._max - bbox._min;
        
        for(size_t i = 0; i < _vertices.size(); i++) {
            _vertices[i] *= dim;
        }
    }

    template<typename base_t>
    void polyhedron<base_t>::scale(const glm::vec<3, base_t> &dim) {
        benchmark::timer tmp("scale()");
        const bbox<base_t> bbox = bounding_box();
        const glm::vec<3, base_t> cur_size = bbox._max - bbox._min;

        for(size_t i = 0; i < _vertices.size(); i++) {
            _vertices[i] *= dim;
        }
    }
    
    template<typename base_t>
    void polyhedron<base_t>::normalize() {
        constexpr bool is_flt = std::is_floating_point<base_t>::value;
        static_assert(is_flt, "normalize(): type must be floating point");
    
        benchmark::timer tmp("normalize()");
        const bbox<base_t> bbox = bounding_box();
        const glm::vec<3, base_t> cur_size = bbox._max - bbox._min;
        const base_t comp_max = glm::compMax(cur_size);
                
        // constexpr functor to calc the new position of the vertex
        auto calc_pos = [=](const glm::vec<3,base_t> &p) constexpr {
            return (p - bbox._min) / comp_max;
        };

        for(size_t i = 0; i < _vertices.size(); i++) {
            _vertices[i] = calc_pos(_vertices[i]);
        }
    }

    template<typename base_t>
    polyhedron<base_t> polyhedron<base_t>::scaled(const polyhedron<base_t> &in, const base_t dim) {
        polyhedron<base_t> res = in;
        res.scale(dim);
        return res;
    }
    
    template<typename base_t>
    polyhedron<base_t> polyhedron<base_t>::scaled(const polyhedron<base_t> &in, const glm::vec<3, base_t> &dim) {
        polyhedron<base_t> res = in;
        res.scale(dim);
        return res;
    }
    
    template<typename base_t>
    polyhedron<base_t> polyhedron<base_t>::normalized(const polyhedron<base_t> &in){
        polyhedron<base_t> res = in;
        res.normalize();
        return res;
    }
    
    template<typename base_t>
    void polyhedron<base_t>::to_obj(const std::string &file) const {
        std::ofstream obj_file(file);
        for(auto v : this->_vertices) {
            obj_file << "v " << v.x << " " << v.y << " " << v.z << std::endl;
        }

        for(auto &f : _indices._buffer) {
            obj_file << "f " << f[0]+1 << " " << f[1]+1 << " " << f[2]+1 << std::endl;
        }
        obj_file.close();
    }

    template<typename base_t>
    bbox<base_t> polyhedron<base_t>::bounding_box() const {
        bbox<base_t> bbox;
        for(const auto &v : this->_vertices) {
            bbox.extend(v);
        }
        return bbox;
    }  
    
    template<typename base_t>
    glm::vec<3, base_t> polyhedron<base_t>::dim() const {
        auto bbox = bounding_box();
        return bbox._max - bbox._min;
    }  
    
    template<typename base_t>
    bool polyhedron<base_t>::issues() const {
        for(const auto &p : _edges) {
            const size_t _face_count = p.second;
            if(_face_count != 2) return false;
        }
        return true;
    }

    template<typename base_t>
    double polyhedron<base_t>::avg_face_area() const {
        auto area = [](const auto &v1, const auto &v2, const auto &v3) {
            double le1 = glm::length(v2-v1);
            double le2 = glm::length(v3-v1);
            double le3 = glm::length(v3-v2);
            double p = 0.5 * (le1 + le2 + le3);
            return std::sqrt(p * (p - le1) * (p - le2) * (p - le3));
        };

        double res = 0;
        const size_t num_triangles = this->_indices._buffer.size();
        for(const auto &id : this->_indices._buffer) {
            const glm::vec3 &v1 = glm::make_vec3(this->_vertices[id._ids[0]]);
            const glm::vec3 &v2 = glm::make_vec3(this->_vertices[id._ids[1]]);
            const glm::vec3 &v3 = glm::make_vec3(this->_vertices[id._ids[2]]);
            double a = area(v1, v2, v3);
            // NaN check
            if(a == a) res += a / num_triangles;
        }
        //std::cout << "avg_face_area(): " << res << std::endl;
        return res;
    }

    template<typename base_t>
    void polyhedron<base_t>::compress() {
        std::map<glm::vec3, uint32_t> unique_vertices;
        for(const auto &v : this->_vertices) {
            unique_vertices[v] = 0;
        }

        uint32_t id = 0;
        for(const auto &p : unique_vertices) {
            unique_vertices[p.first] = id++;
        }

        index_buffer<index_t> new_id_buf; 
        for(auto &f : this->_indices._buffer) {
            const uint32_t id1 = unique_vertices[this->_vertices[f[0]]];
            const uint32_t id2 = unique_vertices[this->_vertices[f[1]]];
            const uint32_t id3 = unique_vertices[this->_vertices[f[2]]];
            new_id_buf.add(mesh::face(id1, id2, id3));
        }

        float before = (float)this->_vertices.size();
        float after = (float)unique_vertices.size();

        std::cout 
            << "Compress result: before=" << before
            << " after=" << after 
            << "; " << (1.f - (after / before)) * 100.f << "%" << std::endl;

        this->_indices.clear();
        this->_indices = new_id_buf;

        this->_vertices.clear();
        for(const auto &p : unique_vertices) {
            this->_vertices.push_back(p.first);
        }
    }
};
