# Cache
C++ Implementation of cache

# BaseCache

Base class is BaseCache, new models should be derived from this class

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


# Base Addr Handler

````C++
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
````

# Base Data Handler

````C++
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
````

# Base Replacer

````C++
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
````
