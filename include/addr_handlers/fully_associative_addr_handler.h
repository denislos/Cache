

#ifndef __PAGRND_CACHE_FULLY_ASSOCIATIVE_ADDR_HANDLER_H_
#define __PAGRND_CACHE_FULLY_ASSOCIATIVE_ADDR_HANDLER_H_

#include "base_addr_handler.h"

namespace pagrnd {
namespace cache {

    template <typename AddrT>
    class FullyAssociativeAddrHandler : public BaseAddrHandler<AddrT, 
                                                               std::size_t, 
                                                               AddrT>
    {
    public:
        using typename BaseAddrHandler<AddrT, std::size_t, AddrT>::SetParamT;
        using typename BaseAddrHandler<AddrT, std::size_t, AddrT>::TagParamT;
        using typename BaseAddrHandler<AddrT, std::size_t, AddrT>::AddrParamT;

        std::size_t get_set([[maybe_unused]]AddrParamT addr) override { return 0; }
        AddrT get_tag(AddrParamT addr) override { return addr; }

        std::pair<std::size_t, AddrT> get_set_tag(AddrParamT addr) override { 
            return std::pair<std::size_t, AddrT>(0, addr);
        }

        AddrT get_addr([[maybe_unused]]SetParamT set, TagParamT tag) override {
            return tag;
        }
    };

} // namespace cache
} // namespace pagrnd

#endif // __PAGRND_CACHE_FULLY_ASSOCIATIVE_ADDR_HANDLER_H_