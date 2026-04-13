#ifndef ENGINE_MATERIALS_PREPROCESSOR_SHADERDEPENDENCYGRAPH_H_
#define ENGINE_MATERIALS_PREPROCESSOR_SHADERDEPENDENCYGRAPH_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "../../project/VirtualPath.hpp"

class ShaderDependencyGraph {
public:
    struct NodeId {
        VirtualPath path;
        std::string section;

        NodeId(const VirtualPath& _path, const std::string& _section);

        bool operator==(const NodeId& other) const;
    
        struct Hasher {
            size_t operator()(const NodeId& id) const noexcept;
        };
    };

    struct NodeData {
        // For future improvements
    };
private:
    std::unordered_map<NodeId, NodeData, NodeId::Hasher> nodesData;
    // DAG - Directed Acyclic Graph 
    std::unordered_map<NodeId, std::vector<NodeId>, NodeId::Hasher> 
        dependencyDAG;
public:
    ShaderDependencyGraph() = default;

    bool nodeExists(const NodeId& node);
    NodeData& getNodeData(const NodeId& node);

    void addNode(const NodeId& node);
    void addDependency(const NodeId& node, const NodeId& dependency);
    void addDependencies(
        const NodeId& node, 
        const std::vector<NodeId>& dependencies
    );

    void removeNode(const NodeId& node);
    void removeDependency(const NodeId& node, const NodeId& dependency);
    void removeDependencies(
        const NodeId& node, 
        const std::vector<NodeId>& dependencies
    );

    std::vector<NodeId> getSortedDependencies(const NodeId& node);
};

#endif // ENGINE_MATERIALS_PREPROCESSOR_SHADERDEPENDENCYGRAPH_H_
