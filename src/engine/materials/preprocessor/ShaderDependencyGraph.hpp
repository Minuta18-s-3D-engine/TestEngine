#ifndef ENGINE_MATERIALS_PREPROCESSOR_SHADERDEPENDENCYGRAPH_H_
#define ENGINE_MATERIALS_PREPROCESSOR_SHADERDEPENDENCYGRAPH_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "../../project/VirtualPath.hpp"

class ShaderDependencyGraph {
public:
    struct Node {
        VirtualPath path;
        std::string section;

        Node() = default;
        Node(const VirtualPath& _path, const std::string& _section)
            : path(_path), section(_section) {}

        bool operator==(const Node& other) const {
            return path.getVirtual() == other.path.getVirtual() &&
                   section == other.section;
        }

        std::string toString() const {
            return path.getVirtual() + section;
        }
    };

    struct NodeHash {
        size_t operator()(const Node& node) const {
            std::hash<std::string> hasher;
            return hasher(node.toString());
        }
    };
private:
    std::unordered_map<Node, std::vector<Node>> dependencyTree;
public:
    ShaderDependencyGraph() = default;

    void addNode(const Node& node);
    void addDependency(const Node& node, const Node& dependency);
    void addDependency(const Node& node, const std::vector<Node>& dependency);

    std::vector<Node> getDependencies(const Node& node);
};

#endif // ENGINE_MATERIALS_PREPROCESSOR_SHADERDEPENDENCYGRAPH_H_
