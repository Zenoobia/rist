#pragma once

#include <string>

struct MovieInfo {
    std::string name;
    float rating;
};

#include <Meta.h>

namespace metas {

template <>
inline auto registerMembers<MovieInfo>()
{
    return members(
        member("name", &MovieInfo::name),
        member("rating", &MovieInfo::rating)
    );
}

} // end of namespace meta
