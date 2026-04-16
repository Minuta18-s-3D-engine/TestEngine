#ifndef ENGINE_MATERIALS_PREPROCESSOR_SHADERDEPENDENCYGRAPH_H_
#define ENGINE_MATERIALS_PREPROCESSOR_SHADERDEPENDENCYGRAPH_H_

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <exception>

#include "../../project/VirtualPath.hpp"

class loop_detected : public std::exception {
    std::string message;
public:
    loop_detected(const char* _message) : message(_message) {}
    loop_detected(const std::string& _message) : message(_message) {}

    const char* what() const noexcept { return message.c_str(); }
};

class ShaderDependencyGraph {
public:
    struct NodeId {
        VirtualPath path;

        NodeId(const VirtualPath& _path);

        bool operator==(const NodeId& other) const;
    
        struct Hasher {
            size_t operator()(const NodeId& id) const noexcept;
        };
    };

    struct NodeData {
        // For future improvements
    };

    struct Node {
        NodeData data;
        std::unordered_set<NodeId, NodeId::Hasher> dependencies;
    };
private:
    // DAG - Directed Acyclic Graph 
    std::unordered_map<
        NodeId, Node, NodeId::Hasher
    > dependencyDAG;

    std::string constructNodeName(const NodeId& node) const;

    void makeInvalidNodeException(
        const NodeId& node, 
        const std::string& message
    );

    void makeInvalidDependencyException(
        const NodeId& node,
        const NodeId& dependency, 
        const std::string& message
    );

    enum class DFSVisitedState {
        NotVisited, 
        Visiting, 
        Visited,
    };

    using DfsNodesUsed = std::unordered_map<
        NodeId, DFSVisitedState, NodeId::Hasher>;
    using DfsResult = std::vector<NodeId>;
    void dependencyDFS(
        const NodeId& node, 
        DfsNodesUsed& nodeUsed, 
        DfsResult& dfsResult
    ) const;
public:
    ShaderDependencyGraph() = default;

    bool nodeExists(const NodeId& node) const;
    NodeData& getNodeData(const NodeId& node);
    size_t size() const { return dependencyDAG.size(); };

    void addNode(const NodeId& node);
    void addNode(const NodeId& node, const NodeData& nodeData);
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

    std::vector<NodeId> getSortedDependencies(const NodeId& node) const;
};

#endif // ENGINE_MATERIALS_PREPROCESSOR_SHADERDEPENDENCYGRAPH_H_
