

#ifndef __PAGRND_CACHE_BASE_CACHE_H_
#define __PAGRND_CACHE_BASE_CACHE_H_

#include "base_addr_handler.h"
#include "base_replacer.h"
#include "base_data_handler.h"

#include "common.h"

namespace pagrnd {
namespace cache {

    template <typename AddrT,
              typename SetT,
              typename TagT,
              typename DataT,
              typename AddrHandlerT,
              typename DataHandlerT,
              typename ReplacerT,
              typename ReplInfoT>
    class BaseCache
    {
    public:
        BaseCache(AddrHandlerT* addr_handler, DataHandlerT* data_handler, ReplacerT* replacer)
            : addr_handler(addr_handler)
            , data_handler(data_handler)
            , replacer(replacer)
        { }

        using AddrParamT = typename boost::call_traits<AddrT>::param_type;
        using DataParamT = typename boost::call_traits<DataT>::param_type;

        using ReplInfoParamT = typename boost::call_traits<ReplInfoT>::param_type;

        virtual std::optional<std::reference_wrapper<const DataT>> read(AddrParamT addr)
        {
            const auto& [set, tag] = addr_handler->get_set_tag(addr);

            const auto& repl_info = replacer->get_repl_info(set, tag);

            auto lookup_result = data_handler->read(set, tag, repl_info);

            if (lookup_result)
                replacer->touch(set, tag);

            return lookup_result;            
        }

        virtual void write(AddrParamT addr, DataParamT data)
        {
            const auto& [set, tag] = addr_handler->get_set_tag(addr);

            bool is_full = data_handler->is_full();

            if (is_full)
            {
                const auto& [should_evict, eviction_tag, repl_info] = replacer->evict(set);

                if (should_evict)
                    data_handler->remove(set, eviction_tag, repl_info);
            }

            const auto& repl_info = replacer->get_repl_info(set, tag);

            data_handler->write(set, tag, data, repl_info);
            replacer->touch(set, tag);
        }

        virtual std::size_t get_capacity() const { return data_handler->get_capacity(); }
        virtual std::size_t get_size() const { return data_handler->get_size(); }

        virtual ~BaseCache() = default;
    
    protected:
        AddrHandlerT* addr_handler = nullptr;
        DataHandlerT* data_handler = nullptr;

        ReplacerT* replacer = nullptr;
    };

} // namespace cache
} // namespace pagrnd

#endif // __PAGRND_CACHE_BASE_CACHE_H_