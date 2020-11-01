


#ifndef __PAGRND_CACHE_FULLY_ASSOCIATIVE_DATA_HANDLER_H_
#define __PAGRND_CACHE_FULLY_ASSOCIATIVE_DATA_HANDLER_H_

#include "base_data_handler.h"

namespace pagrnd {
namespace cache {

    template <typename AddrT, typename DataT>
    class FullyAssociativeDataHandler : public BaseDataHandler<std::size_t,
                                                               AddrT,
                                                               std::size_t,
                                                               DataT>
    {
    public:
        explicit FullyAssociativeDataHandler(std::size_t capacity)
            : BaseDataHandler<std::size_t, AddrT, std::size_t, DataT>(capacity)
        {
            addr2data.reserve(capacity);
        }

        using typename BaseDataHandler<std::size_t, AddrT, std::size_t, DataT>::SetParamT;
        using typename BaseDataHandler<std::size_t, AddrT, std::size_t, DataT>::TagParamT;
        using typename BaseDataHandler<std::size_t, AddrT, std::size_t, DataT>::DataParamT;
        
        using typename BaseDataHandler<std::size_t, AddrT, std::size_t, DataT>::ReplInfoParamT;

        virtual std::optional<std::reference_wrapper<const DataT>> read([[maybe_unused]]SetParamT set, TagParamT tag, [[maybe_unused]]ReplInfoParamT) override {
            auto lookup_result = addr2data.find(tag);
            auto is_hit = lookup_result != addr2data.end();

            return is_hit ? std::make_optional(std::cref(lookup_result->second)) : std::nullopt; 
        }

        virtual void write([[maybe_unused]]SetParamT set, TagParamT tag, DataParamT data, [[maybe_unused]]ReplInfoParamT) override {
            addr2data[tag] = data;
        }

        virtual void remove([[maybe_unused]]SetParamT set, TagParamT tag, [[maybe_unused]]ReplInfoParamT repl_info) override
        {
            addr2data.erase(tag);
        }

        virtual std::size_t get_size() const override { return addr2data.size(); }

        virtual ~FullyAssociativeDataHandler() = default;

    protected:
        std::unordered_map<AddrT, DataT> addr2data;
    };

} // namespace cache
} // namespace pagrnd


#endif // __PAGRND_CACHE_FULLY_ASSOCIATIVE_DATA_HANDLER_H_