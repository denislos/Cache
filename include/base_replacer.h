

#ifndef __PAGRND_CACHE_BASE_REPLACER_H_
#define __PAGRND_CACHE_BASE_REPLACER_H_

#include "common.h"

namespace pagrnd {
namespace cache {

    template <typename SetT, typename TagT, typename ReplInfoT>
    class BaseReplacer
    {
    public:
        BaseReplacer() { }

        using SetParamT = typename boost::call_traits<SetT>::param_type;
        using TagParamT = typename boost::call_traits<TagT>::param_type;

        using ReplInfoParamT = typename boost::call_traits<ReplInfoT>::param_type;

        virtual ReplInfoT get_repl_info(SetParamT set, TagParamT tag) = 0; 

        virtual void touch(SetParamT set, TagParamT tag) = 0;
        virtual std::tuple<bool, TagT, ReplInfoT> evict(SetParamT set) = 0;

        virtual ~BaseReplacer() = default;
    };

} // namespace cache
} // namespace pagrnd

#endif // __PAGRND_CACHE_BASE_REPLACER_H_