

#ifndef __PAGRND_CACHE_BASE_ADDR_HANDLER_H_
#define __PAGRND_CACHE_BASE_ADDR_HANDLER_H_

#include "common.h"

namespace pagrnd {
namespace cache {

    template <typename AddrT, typename SetT, typename TagT>
    class BaseAddrHandler
    {
    public:
        BaseAddrHandler() { }

        using AddrParamT = typename boost::call_traits<AddrT>::param_type;
        using SetParamT  = typename boost::call_traits<SetT>::param_type;
        using TagParamT  = typename boost::call_traits<TagT>::param_type;

        virtual SetT get_set(AddrParamT addr) = 0;
        virtual TagT get_tag(AddrParamT addr) = 0;

        virtual std::pair<SetT, TagT> get_set_tag(AddrParamT addr) = 0;

        virtual AddrT get_addr(SetParamT set, TagParamT tag) = 0;

        virtual ~BaseAddrHandler() = default;
    };

} // namespace cache
} // namespace pagrnd


#endif // __PAGRND_CACHE_BASE_ADDR_HANDLER_H_