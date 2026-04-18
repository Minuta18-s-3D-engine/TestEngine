#include "ShaderDependencyGraph.hpp"

using SDG = ShaderDependencyGraph;

inline void hash_combine(std::size_t& seed, std::size_t hash) {
    // view boost implementation 
    seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

SDG::NodeId::NodeId(
    const VirtualPath& _path
) : path(_path) {}

bool SDG::NodeId::operator==(const NodeId& other) const {
    return path.getVirtual() == other.path.getVirtual();
}

size_t SDG::NodeId::Hasher::operator()(const NodeId& id) const noexcept {
    size_t h1 = std::hash<std::string>{}(id.path.getVirtual());

    return h1;
}

std::string SDG::constructNodeName(const SDG::NodeId& node) const {
    return node.path.getVirtual();
}

void SDG::makeInvalidNodeException(
    const SDG::NodeId& node, 
    const std::string& message
) const {
    std::string formattedMessage = "Exception in node " + 
        constructNodeName(node) + ": " + message; 
    throw std::invalid_argument(formattedMessage);
}

void SDG::makeInvalidDependencyException(
    const SDG::NodeId& node,
    const SDG::NodeId& dependency, 
    const std::string& message
) const {
    makeInvalidNodeException(node, " " + constructNodeName(node) + 
        ": " + message
    );  
}

bool SDG::nodeExists(const SDG::NodeId& node) const {
    return dependencyDAG.contains(node);
}

SDG::NodeData& SDG::getNodeData(const SDG::NodeId& node) {
    if (!nodeExists(node)) {
        makeInvalidNodeException(node, "node not exists");
    }
    return dependencyDAG.at(node).data;
}

void SDG::addNode(const NodeId& node) {
    addNode(node, NodeData());
}

void SDG::addNode(const SDG::NodeId& node, const NodeData& nodeData) {
    if (nodeExists(node)) {
        makeInvalidNodeException(node, "node already exists");
    }
    dependencyDAG[node].data = nodeData;
    dependencyDAG[node].dependencies = \
        std::unordered_set<NodeId, NodeId::Hasher>();
}

void SDG::addDependency(const NodeId& node, const NodeId& dependency) {
    if (!nodeExists(node)) {
        makeInvalidNodeException(node, "node not exists");
    }
    dependencyDAG[node].dependencies.insert(dependency);
}

void SDG::addDependencies(
    const NodeId& node, 
    const std::vector<NodeId>& dependencies
) {
    if (!nodeExists(node)) {
        makeInvalidNodeException(node, "node not exists");
    }

    for (auto& n : dependencies) {
        if (!nodeExists(n)) {
            makeInvalidNodeException(node, "node not exists");
        }
    }

    for (auto& n : dependencies) {
        dependencyDAG[node].dependencies.insert(n);
    }
}

void SDG::removeNode(const NodeId& node) {
    if (!nodeExists(node)) {
        makeInvalidNodeException(node, "node not exists");
    }
    dependencyDAG.erase(node);
}

void SDG::removeDependency(const NodeId& node, const NodeId& dependency) {
    if (!nodeExists(node)) {
        makeInvalidNodeException(node, "node not exists");
    }
    if (!dependencyDAG.contains(dependency)) {
        makeInvalidDependencyException(
            node, dependency, "dependency not exists"
        );
    }
    dependencyDAG[node].dependencies.erase(dependency);
}

void SDG::removeDependencies(
    const NodeId& node, 
    const std::vector<NodeId>& dependencies
) {
    if (!nodeExists(node)) {
        makeInvalidNodeException(node, "node not exists");
    }

    for (auto& n : dependencies) {
        if (!dependencyDAG[node].dependencies.contains(n)) {
            makeInvalidDependencyException(
                node, n, "dependency not exists"
            );
        }
    }

    for (auto& n : dependencies) {
        dependencyDAG[node].dependencies.erase(n);
    }
}

void SDG::dependencyDFS(
    const NodeId& node, 
    DfsNodesUsed& nodeUsed, 
    DfsResult& dfsResult
) const {
    nodeUsed[node] = DFSVisitedState::Visiting;
    for (const auto& dependency : dependencyDAG.at(node).dependencies) {
        if (!nodeUsed.contains(dependency)) {
            if (nodeUsed[dependency] == DFSVisitedState::Visiting) {
                throw loop_detected(
                    "Circular dependency found: " + constructNodeName(node) + 
                    "->" + constructNodeName(dependency)
                );
            }
            if (nodeUsed[dependency] == DFSVisitedState::Visited) {
                continue;
            }
        }
        dependencyDFS(dependency, nodeUsed, dfsResult);
    }
    nodeUsed[node] = DFSVisitedState::Visited;
    dfsResult.push_back(node);
}

std::vector<SDG::NodeId> SDG::getSortedDependencies(
    const NodeId& node, bool reverse
) const {
    if (!nodeExists(node)) {
        makeInvalidNodeException(node, "node not exists");
    }

    DfsNodesUsed nodeUsed;
    DfsResult dfsResult;
    dependencyDFS(node, nodeUsed, dfsResult);
    if (reverse) {
        std::reverse(dfsResult.begin(), dfsResult.end());
    }
    return dfsResult;
}
