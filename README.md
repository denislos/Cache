# Cache
C++ Implementation of cache

# BaseCache

Base class is BaseCache, new models should be derived from this class

````C++
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
````

Such interface allows to use the same cache business logic with multiple models (fully-associative, direct-mapped, set-associative cache, segmented caches, etc).
Furthemore, extremely complex cache structures can be implemented due to versatility of handler classes


# Base Addr Handler

This class is used to generate abstract (Set, Tag) from Addr

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

This class manages how data is stored in a cache

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

This class is used to define replacement policy

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
