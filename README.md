# Cache
C++ Implementation of cache

Base class is BaseCache

````C++
    template <typename AddrT,        // type of address
              typename SetT,         // type of abstract set
              typename TagT,         // type of abstract set
              typename DataT,        // type of abstract data
              typename AddrHandlerT, // type of addr handler (addr -> (set, tag))
              typename DataHandlerT, // abstract interface for accessing data (can be the same DataHandler for multiple replacers)
              typename ReplacerT,    // abstract replacer interface (can be the same replacer for multiple data handler)
              typename ReplInfoT>    // abstract replacement info (can be used in Segmented caches)
    class BaseCache
````

Such interface allows to use the same cache business logic with multiple models (fully-associative, direct-mapped, set-associative cache, segmented caches, etc).
Furthemore, extremely complex cache structures can be implemented due to versatility of handler classes
