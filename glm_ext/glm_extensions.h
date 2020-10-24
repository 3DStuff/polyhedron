#pragma once

// we do not want to change glm 
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-volatile"

    //#define GLM_FORCE_SSE2 
    //#define GLM_FORCE_AVX
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

#pragma GCC diagnostic pop

template <typename vec_t>
struct compare_glm {
    bool operator() (const vec_t& lhs, const vec_t& rhs) const {
        return glm::all(glm::lessThan(lhs, rhs));
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
