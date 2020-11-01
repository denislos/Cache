

#ifndef __PAGRND_CACHE_LFU_CACHE_H_
#define __PAGRND_CACHE_LFU_CACHE_H_

#include "base_cache.h"
#include "fully_associative_addr_handler.h"
#include "fully_associative_data_handler.h"
#include "lfu_replacer.h"

namespace pagrnd {
namespace cache {

    template <typename AddrT, typename DataT>
    class LFUFullyAssociativeCache : public BaseCache<AddrT,
                                                      std::size_t,
                                                      std::size_t,
                                                      DataT,
                                                      FullyAssociativeAddrHandler<AddrT>,
                                                      FullyAssociativeDataHandler<AddrT, DataT>,
                                                      LFUReplacer<AddrT>,
                                                      std::size_t>
    {
    public:
        LFUFullyAssociativeCache(FullyAssociativeAddrHandler<AddrT>* addr_handler,
                                 FullyAssociativeDataHandler<AddrT, DataT>* data_handler,
                                 LFUReplacer<AddrT>* replacer)
            : BaseCache<AddrT,
                        std::size_t,    
                        std::size_t,
                        DataT,
                        FullyAssociativeAddrHandler<AddrT>,
                        FullyAssociativeDataHandler<AddrT, DataT>,
                        LFUReplacer<AddrT>,
                        std::size_t>(addr_handler, data_handler, replacer)
        { }
     };

} // namespace cache
} // namespace pagrnd

#endif // __PAGRND_CACHE_LFU_CACHE_H_