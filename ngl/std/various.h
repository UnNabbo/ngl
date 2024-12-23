namespace std // NOTE(Nabbo): HACK TO USE STD FUCTIONALITY WITHOUT STD
{
    template<typename T>
	struct initializer_list
    {		
        constexpr initializer_list() noexcept : First(nullptr), Last(nullptr) {}
		
        constexpr initializer_list(const T* First, const T* Last) noexcept
            : First(First), Last(Last) {}
		
        // First element.
        constexpr const T* begin() const noexcept { return First; }
		
        // One past the last element.
        constexpr const T* end() const noexcept { return Last; }
		
        // Number of elements.
        constexpr size_t size() const noexcept
        {
            return static_cast<size_t>(Last - First);
        }
		
        const T* First;
        const T* Last;
    };
	
	template<typename T>
		constexpr const T* begin(initializer_list<T> il) noexcept 
    {
        return il.begin();
    }
	
    template<typename T>
		constexpr const T* end(initializer_list<T> il) noexcept
    {
        return il.end();
    }
}

template<typename T>
using init_list = std::initializer_list<T>;
