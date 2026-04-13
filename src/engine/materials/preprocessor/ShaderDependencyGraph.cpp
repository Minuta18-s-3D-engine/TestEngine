#include "ShaderDependencyGraph.hpp"

inline void hash_combine(std::size_t& seed, std::size_t hash) {
    // view boost implementation 
    seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

ShaderDependencyGraph::NodeId::NodeId(
    const VirtualPath& _path,
    const std::string& _section
) : path(_path), section(_section) {}

bool ShaderDependencyGraph::NodeId::operator==(
    const NodeId& other
) const {
    return path.resolve() == other.path.resolve() &&
        section == other.section;
}

size_t ShaderDependencyGraph::NodeId::Hasher::operator()(
    const NodeId& id
) const noexcept {
    size_t h1 = std::hash<std::string>{}(id.path.resolve());
    size_t h2 = std::hash<std::string>{}(id.section);

    size_t h = h1;
    hash_combine(h, h2);
    return h;
}

