
#include <cstdlib>
#include <iostream>
#include <vector>

#include "lfu_cache.h"


using AddrHandlerT = pagrnd::cache::FullyAssociativeAddrHandler<std::size_t>;
using DataHandlerT = pagrnd::cache::FullyAssociativeDataHandler<std::size_t, std::size_t>;
using ReplacerT    = pagrnd::cache::LFUReplacer<std::size_t>;

using CacheT       = pagrnd::cache::LFUFullyAssociativeCache<std::size_t, std::size_t>;

int main()
{
    std::size_t capacity = 0;
    std::size_t num_elems = 0;

    std::cin >> capacity;
    std::cin >> num_elems;

    AddrHandlerT addr_handler = AddrHandlerT();
    DataHandlerT data_handler = DataHandlerT(capacity);
    ReplacerT replacer = ReplacerT();

    CacheT cache = CacheT(&addr_handler, &data_handler, &replacer);

    std::vector<std::size_t> access_pattern(num_elems);

    for (auto& elem : access_pattern)
        std::cin >> elem;

    const std::size_t def_data = 0xBADF00D;

    std::size_t num_hits = 0;

    for (const auto addr : access_pattern) {
        auto read_result = cache.read(addr);
        
        bool is_hit = read_result.has_value();

        if (!is_hit)
            cache.write(addr, def_data);
        else
            num_hits++;
    }

    std::cout << "Num hits: " << num_hits << std::endl;

    return EXIT_SUCCESS;
}