

#ifndef __PAGRND_CACHE_LFU_REPLACER_H_
#define __PAGRND_CACHE_LFU_REPLACER_H_

#include "base_replacer.h"

#include <unordered_map>
#include <map>

namespace pagrnd {
namespace cache {

    template <typename AddrT>
    class LFUReplacer : public BaseReplacer<std::size_t, AddrT, std::size_t>
    {
    public:
        LFUReplacer()
            : BaseReplacer<std::size_t, AddrT, std::size_t>()
        { }

        using typename BaseReplacer<std::size_t, AddrT, std::size_t>::SetParamT;
        using typename BaseReplacer<std::size_t, AddrT, std::size_t>::TagParamT;

        std::size_t get_repl_info([[maybe_unused]]SetParamT set, [[maybe_unused]]TagParamT tag) override { return 0; }

        void touch([[maybe_unused]]SetParamT set, TagParamT tag) override {
            auto lookup_result = addr2freq_index.find(tag);

            if (lookup_result == addr2freq_index.end())
            {
                auto freq_index = freq2addr.emplace(0, tag);
                addr2freq_index.emplace(tag, freq_index);
                return;
            }

            auto freq_index = lookup_result->second;
            const auto current_frequency = freq_index->first;

            freq2addr.erase(freq_index);
            lookup_result->second = freq2addr.emplace(current_frequency + 1, tag);
        }

        std::tuple<bool, AddrT, std::size_t> evict([[maybe_unused]]SetParamT set) override {
            const auto eviction_it = freq2addr.begin();
            const auto eviction_tag = eviction_it->second;

            addr2freq_index.erase(eviction_tag);
            freq2addr.erase(eviction_it);

            return std::make_tuple(true, eviction_tag, get_repl_info(set, eviction_tag));
        }
    
    private:
        using FreqIndexT = typename std::multimap<std::size_t, AddrT>::iterator;

        std::unordered_map<AddrT, FreqIndexT> addr2freq_index;
        std::multimap<std::size_t, AddrT> freq2addr;
    };    

} // namespace cache
} // namespace pagrnd

#endif // __PAGRND_CACHE_LFU_REPLACER_H_