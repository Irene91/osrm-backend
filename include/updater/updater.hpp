#ifndef OSRM_UPDATER_UPDATER_HPP
#define OSRM_UPDATER_UPDATER_HPP

#include "updater/updater_config.hpp"

#include "extractor/edge_based_edge.hpp"

#include <chrono>
#include <vector>

namespace osrm
{
namespace updater
{
class Updater
{
  public:
    Updater(UpdaterConfig config_) : config(std::move(config_)) {}

    using NodesAndEdges = std::tuple<std::vector<extractor::EdgeBasedNodeData>,
                                     std::vector<extractor::EdgeBasedEdge>,
                                     std::uint32_t>;
    NodesAndEdges LoadAndUpdateEdgeExpandedGraph() const;

    void
    LoadAndUpdateEdgeExpandedGraph(std::vector<extractor::EdgeBasedNodeData> &edge_based_node_list,
                                   std::vector<extractor::EdgeBasedEdge> &edge_based_edge_list,
                                   std::uint32_t &connectivity_checksum) const;

  private:
    UpdaterConfig config;
};
}
}

#endif
