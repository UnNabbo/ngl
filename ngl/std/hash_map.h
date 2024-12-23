#define FIRST_AVAILABLE_HASH 0x000000000ff
#define HASH_MAP_FULL 0xFF
#define HASH_MAP_EMPTY 0x00
#define HASH_TABLE_START_SIZE 64

template<typename T, typename U>
struct hash_pair{
	u8 BitField;
	T Key;
	U Value;
};

template<typename T, typename U>
struct hash_map{
	hash_pair<T, U> * Table; 
	u64 ReservedSize;
	
	u64 Size;
	
	u64 (*HashFunction)(T Key) = FNVHash;
	
	U * operator[](T Key){
		if(!this->ReservedSize){
			return 0;
		}
		
		u64 Hash = this->HashFunction(Key);
		
		u32 ProbeIncrement = 1;
		
		u32 Index = Hash % this->ReservedSize;
		while(hash_pair<T, U>* Slot = &this->Table[Index]){
			if(Slot->BitField == HASH_MAP_EMPTY){
				break;
			}else if(Slot->Key == (T)Key){
				return &Slot->Value;
			}
			
			Index = (Index + ProbeIncrement++) % this->ReservedSize;
		}
		return 0;
	}
};

template<typename T, typename U>
void HashMapReHash(hash_map<T, U>* HashMap, hash_pair<T, U>* Table){
	if(HashMap->Size < 2){
		return;
	}
	
	for(int i = 0; i < HashMap->Size; i++){
		u32 ProbeIncrement = 1;
		u32 Index = HashMap->HashFunction(HashMap->Table[i].Key) % HashMap->ReservedSize;
		while(hash_pair<T, U>* Slot = &Table[Index]){
			if(Slot->BitField == HASH_MAP_EMPTY){
				Table[Index] = HashMap->Table[i];
				break;
			}
			
			Index = (Index + ProbeIncrement++) % HashMap->ReservedSize;
		}
		
	}
}

template<typename T, typename U>
void HashMapGrowTable(hash_map<T, U>* HashMap, u64 Size){
	Assert(Size > HashMap->ReservedSize, "Could not grow table: % > %", Size, HashMap->ReservedSize);
	HashMap->ReservedSize = Size;
	if(HashMap->Table){
		hash_pair<T, U>* NewTable = (hash_pair<T, U>*)MemAlloc(Size * sizeof(hash_pair<T, U>));
		HashMapReHash(HashMap, NewTable);
		MemFree(HashMap->Table);
		HashMap->Table = NewTable;
	}else{
		HashMap->Table = (hash_pair<T, U>*)MemAlloc(Size * sizeof(hash_pair<T, U>));
	}
}

template<typename T, typename U>
void HashMapShrinkTable(hash_map<T, U>* HashMap, u64 Size){
	Assert(HashMap->Table && Size < HashMap->ReservedSize, "Could not shrink table");
	HashMap->Table = MemRealloc(HashMap->Table, Size);
	HashMap->ReservedSize = Size;
	HashMapReHash(HashMap);
}

template<typename T, typename U>
hash_map<T, U> HashMapNew(u64 (*HashFunction)(T) = FNVHash){
	hash_map<T, U> HashMap = {};
	
	HashMapGrowTable(&HashMap, HASH_TABLE_START_SIZE);
	HashMap.HashFunction = HashFunction;
	
	return HashMap;
}

template<typename T, typename U>
void HashMapFree(hash_map<T, U>  * HashMap){
	MemFree(HashMap->Table);
	ZeroData(HashMap);
}

template<typename T, typename U, typename P, typename K>
U* HashMapInsert(hash_map<T, U>* HashMap, P Key, K Value){
	if(HashMap->Size + 16 > HashMap->ReservedSize){
		HashMapGrowTable(HashMap, (HashMap->ReservedSize) ? HashMap->ReservedSize * 4 : HASH_TABLE_START_SIZE);
	}
	u64 Hash = HashMap->HashFunction(Key);
	
	u32 ProbeIncrement = 1;
	
	u32 Index = Hash % HashMap->ReservedSize;
	while(hash_pair<T, U>* Slot = &HashMap->Table[Index]){
		if(Slot->BitField == HASH_MAP_EMPTY){
			Slot->BitField = HASH_MAP_FULL;
			Slot->Key = Key;
			Slot->Value = Value;
			HashMap->Size++;
			
			return &Slot->Value;
		}else if(Slot->Key == (T)Key){
			break;
		}
		
		Index = (Index + ProbeIncrement++) % HashMap->ReservedSize;
	}
	return 0;
}

template<typename T, typename U, typename P>
U * HashMapInsert(hash_map<T, U>* HashMap, P Key){
	return HashMapInsert(HashMap, Key, U{});
}

template<typename T, typename U, typename P>
U* HashMapFind(hash_map<T, U>* HashMap, P Key){
	u64 Hash = HashMap->HashFunction(Key);
	
	u32 ProbeIncrement = 1;
	
	u32 Index = Hash % HashMap->ReservedSize;
	while(hash_pair<T, U>* Slot = &HashMap->Table[Index]){
		if(Slot->BitField == HASH_MAP_EMPTY){
			return 0;
		}else if(Slot->Key == (T)Key){
			return &Slot->Value;
		}
		Index = (Index + ProbeIncrement++) % HashMap->ReservedSize;
	}
	
}
