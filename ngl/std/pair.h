#if 1
template<typename T, typename U>
struct pair{
	T First;
	U Second;
};

// Todo - remove the pair, use pointers instead
// like `str out = StringSplit(str input, char token, str &out);
pair<string, string> StringSplit(string String, char SplitToken){
	s32 i = 0;
	for(; String[i] != SplitToken; i++){
		if(i >= String.Size){
			return {{}, String};
		}
	}
	
	char * String1 = (char*) calloc(1, i + 1);
	memcpy(String1, String.Data, i);
	char * String2 = (char*) calloc(1, String.Size - i + 1);
	memcpy(String2, String.Data + i, String.Size - i);
	
	return pair<string, string>(string{String1, i}, string{String2, String.Size - i});
}
#endif

template<typename T, typename U>
s32 ArrayPairLinearSearch(array<pair<T, U>>* arr, T item) {
	for(int i = 0; i < arr->Size; i++) {
		if(arr->Base[i].First == item) {
			return i;
		}
	}
	return -1;
}

template<typename T, typename U>
U* ArrayPairAddIfUnique(array<pair<T, U>>* arr, pair<T, U> p) {
	s32 idx = ArrayPairLinearSearch(arr, p.First);
	if(idx != -1) return &arr->Base[idx].Second;
	
	U item = {};
	p.Second = item;
	ArrayAppend(arr, p);
	return &arr->Base[arr->Size - 1].Second;
}
