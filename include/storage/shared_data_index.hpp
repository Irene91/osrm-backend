#ifndef OSRM_STORAGE_SHARED_DATA_INDEX_HPP
#define OSRM_STORAGE_SHARED_DATA_INDEX_HPP

#include "storage/shared_datatype.hpp"

#include <boost/function_output_iterator.hpp>

#include <unordered_map>

namespace osrm
{
namespace storage
{

// This class wraps one or more shared memory regions with the associated data layout
// to abstract away in which region a block of memory is stored.
class SharedDataIndex
{
  public:
    struct AllocatedRegion
    {
        char *memory_ptr;
        DataLayout layout;
    };

    SharedDataIndex() = default;
    SharedDataIndex(std::vector<AllocatedRegion> regions_) : regions(std::move(regions_))
    {
        // Build mapping from block name to region
        for (auto index : util::irange<std::uint32_t>(0, regions.size()))
        {
            regions[index].layout.List("",
                                       boost::make_function_output_iterator([&](const auto &name) {
                                           block_to_region[name] = index;
                                       }));
        }
    }

    template <typename OutIter> void List(const std::string &name_prefix, OutIter out) const
    {
        for (const auto &region : regions)
        {
            region.layout.List(name_prefix, out);
        }
    }

    template <typename T> auto GetBlockPtr(const std::string &name) const
    {
        const auto index_iter = block_to_region.find(name);
        const auto &region = regions[index_iter->second];
        return region.layout.GetBlockPtr<T>(region.memory_ptr, name);
    }

    template <typename T> auto GetBlockPtr(const std::string &name)
    {
        const auto index_iter = block_to_region.find(name);
        const auto &region = regions[index_iter->second];
        return region.layout.GetBlockPtr<T>(region.memory_ptr, name);
    }

    std::size_t GetBlockEntries(const std::string &name) const
    {
        const auto index_iter = block_to_region.find(name);
        const auto &region = regions[index_iter->second];
        return region.layout.GetBlockEntries(name);
    }

    std::size_t GetBlockSize(const std::string &name) const
    {
        const auto index_iter = block_to_region.find(name);
        const auto &region = regions[index_iter->second];
        return region.layout.GetBlockSize(name);
    }

  private:
    std::vector<AllocatedRegion> regions;
    std::unordered_map<std::string, std::uint32_t> block_to_region;
};
}
}

#endif
