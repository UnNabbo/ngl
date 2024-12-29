template<typename T>
struct ring{
	T* Memory;
	u32 Size;
	u32 Offset;
	
	T& operator[](u32 Index){
		return Memory[abs(Offset + Index) % Size];
	}
};

template<typename T>
ring<T> RingCreate(u32 Size){
	ring<T> Ring = {};
	Ring.Size = Size;
	Ring.Offset = 0;
	Ring.Memory = (T*)MemAlloc(Size * sizeof(T));
	return Ring;
}

template<typename T>
T& RingAppend(ring<T> *Ring, T Item){
	Assert(Ring->Size != 0, "Ring isn't inizialized!");
	(*Ring)[Ring->Size] = Item;
	Ring->Offset++;
	return (*Ring)[Ring->Size - 1];
}


