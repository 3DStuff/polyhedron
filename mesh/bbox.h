#pragma once
 
#include "../glm_ext/glm_extensions.h"


namespace mesh {
    template<typename base_t>
    struct bbox {
        glm::vec<3, base_t> _min = glm::vec3(FLT_MAX);
        glm::vec<3, base_t> _max = glm::vec3(-FLT_MAX);

        // bbox valid or broken?
        // fails if any axis zero or FLT_MAX
        bool valid() const {
            if(glm::any(glm::lessThanEqual(glm::abs(_max-_min), glm::vec3(FLT_EPSILON)))) return false;
            if(glm::any(glm::equal(glm::abs(_min), glm::vec3(FLT_MAX)))) return false;
            if(glm::any(glm::equal(glm::abs(_max), glm::vec3(FLT_MAX)))) return false;
            return true;
        }

        glm::vec<3, base_t> dim() const {
            return _max - _min;
        }

        //! Estimates global factor to scale the whole model to 0 < x < 1
        base_t scale() const {
            base_t max(-FLT_MAX);
            glm::vec3 dif = _max - _min;

            for(int i = 0; i < 3; i++) {
                max = dif[i] > max ? dif[i] : max;
            }
            return base_t(1) / max;
        }

        //! Estimates offset to scale the whole model to 0 <= x <= 1
        glm::vec<3, base_t> offset() const {
            return _min * base_t(-1);
        }

        //! For iterating over vertex lists
        //! pass a vertex and calculate the bounding box on the fly
        void extend(const glm::vec<3, base_t> &v) {
            _min = glm::min(_min, v);
            _max = glm::max(_max, v);
        }

        void extend(const bbox<base_t> &other) {
            _min = glm::min(_min, other._min);
            _max = glm::max(_max, other._max);
        }
    };
};