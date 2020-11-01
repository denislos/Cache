

#ifndef __PAGRND_CACHE_BASE_DATA_HANDLER_H_
#define __PAGRND_CACHE_BASE_DATA_HANDLER_H_

#include "common.h"

#include <optional>
#include <functional>

namespace pagrnd {
namespace cache {

    template <typename SetT, typename TagT, typename ReplInfoT, typename DataT>
    class BaseDataHandler
    {
    public:
        explicit BaseDataHandler(std::size_t capacity)
            : capacity(capacity)
        { }

        using DataParamT = typename boost::call_traits<DataT>::param_type;
        using SetParamT  = typename boost::call_traits<SetT>::param_type;
        using TagParamT  = typename boost::call_traits<TagT>::param_type;
        
        using ReplInfoParamT = typename boost::call_traits<ReplInfoT>::param_type;

        virtual std::optional<std::reference_wrapper<const DataT>> read(SetParamT set, 
                                                                        TagParamT tag, 
                                                                        ReplInfoParamT repl_info) = 0;
        virtual void write(SetParamT set, TagParamT tag, DataParamT data, ReplInfoParamT repl_info) = 0;

        virtual void remove(SetParamT set, TagParamT tag, ReplInfoParamT repl_info) = 0;

        virtual std::size_t get_size() const = 0;
        virtual std::size_t get_capacity() const { return capacity; }

        virtual bool is_full() const { return get_capacity() == get_size(); }

        virtual ~BaseDataHandler() = default;

    protected:
        std::size_t capacity = 0;
    };

} // namespace cache
} // namespace pagrnd

#endif // __PAGRND_CACHE_BASE_DATA_HANDLER_H_