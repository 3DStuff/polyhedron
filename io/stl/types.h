#pragma once

#include "../../glm_ext/glm_extensions.h"

#include <algorithm>
#include <array>


namespace stl {
    #pragma pack(push, 1)
    struct face {
        glm::vec3 _norm;
        glm::vec3 _vert_1;
        glm::vec3 _vert_2;
        glm::vec3 _vert_3;
        uint16_t _attribute = 0;

        face() = default;
        face(glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, const glm::vec3 &n = glm::vec3(0)) {
            _vert_1 = p1;
            _vert_2 = p2;
            _vert_3 = p3;

            if(glm::length(n) < std::numeric_limits<float>::epsilon()) {
                const glm::vec3 u = _vert_2 - _vert_1;
                const glm::vec3 v = _vert_3 - _vert_1;
                _norm = glm::cross(u, v);
            }
            else {
                _norm = n;
            }
        }

        friend bool operator < (const face &lhs, const face &rhs) {
            std::array<glm::vec3, 3> larr = { lhs._vert_1, lhs._vert_2, lhs._vert_3 };
            std::sort(larr.begin(), larr.end());

            std::array<glm::vec3, 3> rarr = { rhs._vert_1, rhs._vert_2, rhs._vert_3 };
            std::sort(rarr.begin(), rarr.end());

            return larr < rarr;
        }
    };
    #pragma pack(pop)
}