#pragma once

// we do not want to change glm
#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-volatile"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#endif
    #define GLM_FORCE_SWIZZLE 
    #define GLM_ENABLE_EXPERIMENTAL

    #include <glm/glm.hpp>
    #include <glm/gtx/closest_point.hpp>
    #include <glm/gtx/intersect.hpp>
    #include <glm/gtc/epsilon.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtx/projection.hpp>
    #include <glm/gtx/vector_angle.hpp>
    #include <glm/gtx/transform.hpp>
    #include <glm/gtx/component_wise.hpp>
    #include <glm/gtc/type_ptr.hpp>
#ifdef __clang__
#pragma GCC diagnostic pop
#endif

#include <tuple>

namespace glm {
    template <class base_t>
    bool operator==(const glm::vec<3, base_t> &lhs, const glm::vec<3, base_t> &rhs) { 
        return 
            lhs.x - rhs.x <= std::numeric_limits<base_t>::epsilon() &&
            lhs.y - rhs.y <= std::numeric_limits<base_t>::epsilon() &&
            lhs.z - rhs.z <= std::numeric_limits<base_t>::epsilon();
    }

    template <class base_t>
    bool operator!=(const glm::vec<3, base_t> &lhs, const glm::vec<3, base_t> &rhs) { 
        return !(lhs == rhs);
    }
    
    template <class base_t>
    bool operator<(const glm::vec<3, base_t> &lhs, const glm::vec<3, base_t> &rhs) { 
        return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
    }
};

template <typename base_t>
base_t constrain(const base_t &min, const base_t &max, const base_t &val) {
    return val < min ? min : val > max ? max : val;
}

template <typename base_t>
glm::vec<3, base_t> constrain(const glm::vec<3, base_t> &min, const glm::vec<3, base_t> &max, glm::vec<3, base_t> &val) {
    return {
        constrain(min.x, max.x, val.x),
        constrain(min.y, max.y, val.y),
        constrain(min.z, max.z, val.z)
    };
}
