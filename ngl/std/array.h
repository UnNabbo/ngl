template <typename T>
struct array {
    T* Base;
    s32 Size;
    s32 ReservedSize;

	array() = default;

    T& operator[](s32 i) {AssertBounds(i, 0, Size); return Base[i]; }
	operator T*(){ return Base; }
};

template<typename T>
array<T> ArrayNew(s32 Size) {
	Assert(Size >= 0, "Unable to create an array with negative size %", Size);
    array<T> Array = {};
    Array.ReservedSize = Size;
    Array.Size = 0;
    Array.Base = (T*)MemAlloc(Size * sizeof(T));
    return Array;
}

template<typename T>
array<T> ArrayNew(init_list<T> List){
	s32 Size = List.Last - List.First;

	array<T> Array = {};
	Array.ReservedSize = Size;
	Array.Size = Size;
	Array.Base = (T*)MemAlloc(Size * sizeof(T));

	s32 i = 0;
	for(auto& Item : List){
		Array[i++] = Item;
	}

	return Array;
}

template<typename T>
void ArrayResize(array<T>* Array, s32 NewSize) {
	if(!Array->Base){
		Array->Base = (T*)MemAlloc(sizeof(T) * NewSize);
	}else{
		Array->Base = (T*)MemRealloc(Array->Base, sizeof(T) * NewSize);
	}
    Array->ReservedSize = NewSize;
}

template<typename T>
void ArrayReserve(array<T>* Array, s32 ToAdd) {
    if(Array->Size + ToAdd <= Array->ReservedSize) return;

    s32 NewSize = ((Array->ReservedSize * 2) > (Array->ReservedSize + ToAdd)) ? Array->ReservedSize * 2 : Array->ReservedSize + ToAdd;
    ArrayResize(Array, NewSize);
}

template<typename T>
void ArrayClean(array<T>* Array) {
    Array->Size = 0;
}

template<typename T>
T& ArrayAppend(array<T>* Array, T Data) {
    if (Array->Size + 1 > Array->ReservedSize) {
		Array->ReservedSize = (!Array->ReservedSize) ? 8 : Array->ReservedSize;
        ArrayResize(Array, Array->ReservedSize * 2);
    }

    Array->Base[Array->Size] = Data;
	return Array->Base[Array->Size++];
}

template<typename T>
T& ArrayPush(array<T>* Array, T Data) {
    return ArrayAppend(Array, Data);
}

template<typename T>
T& ArrayAppend(array<T>* Array) {
    if (Array->Size + 1 > Array->ReservedSize) {
		Array->ReservedSize = (!Array->ReservedSize) ? 8 : Array->ReservedSize;
        ArrayResize(Array, Array->ReservedSize * 2);
    }

    Array->Base[Array->Size] = {};
	return Array->Base[Array->Size++];
}

template<typename T>
T& ArrayInsert(array<T>* Array, s32 Index, T Base) {
    if (Array->Size + 1 > Array->ReservedSize) {
		Array->ReservedSize = (!Array->ReservedSize) ? 8 : Array->ReservedSize;
        ArrayResize(Array, Array->ReservedSize * 16);
    }

	for(int i = Array->Size; i >= Index; i--){
		Array->Base[i + 1] = Array->Base[i];
	}
    Array->Base[Index] = Base;
	Array->Size++;

	return Array->Base[Index];
}

template<typename T>
void ArrayRemove(array<T>* Array, s32 Index) {
    if (Array->Size + 1 > Array->ReservedSize) {
		Array->ReservedSize = (!Array->ReservedSize) ? 8 : Array->ReservedSize;
        ArrayResize(Array, Array->ReservedSize * 2);
    }

	for(int i = Index; i < Array->Size ; i++){
		Array->Base[i] = Array->Base[i + 1];
	}
	Array->Size--;
}

template<typename T>
T& ArrayPop(array<T>* Array) {
    return Array->Base[--Array->Size];
}

template<typename T>
void ArrayShift(array<T>* Array, u32 Amount, s32 Pivot = 0) {
    if (Array->Size + Amount > Array->ReservedSize) {
		Array->ReservedSize = (!Array->ReservedSize) ? 8 : Array->ReservedSize;
        ArrayResize(Array, Array->ReservedSize * 2);
    }

	for(s32 i = Array->Size; i >= Pivot; i--){
		Array->Base[i + Amount] = Array->Base[i];
	}
	Array->Size += Amount;
}

template<typename T>
void ArrayFree(array<T>* Array) {
	if(Array->Base){
		MemFree(Array->Base);
		Array->Size = 0;
		Array->ReservedSize = 0;
	}
}

template<typename T>
struct static_array{
	T * Data;
	u32 Size;
	T& operator[](int Index){ return ((T*)Data)[Index]; }
};

template<typename T>
static_array<T> StaticArrayNew(u32 Size){
	static_array<T> Arr;
	Arr.Data = (T*)MemAlloc(Size * sizeof(T));
	Arr.Size = Size;
	return Arr;
}

#ifdef DEBUG
#pragma optimize("g", on)
#endif

template<typename T>
void MergeArrayInPlace(array<T>* Arr, int Start, int Mid, int End)
{
    int Start2 = Mid + 1;

    // If the direct merge is already sorted
    if ((*Arr)[Mid] <= (*Arr)[Start2]) {
        return;
    }

    while (Start <= Mid && Start2 <= End) {

        // If element 1 is in right place
        if ((*Arr)[Start] <= (*Arr)[Start2]) {
            Start++;
        }
        else {
            T value = (*Arr)[Start2];
            int index = Start2;

            // Shift all the elements between element 1
            // element 2, right by 1.
            while (index != Start) {
                (*Arr)[index] = (*Arr)[index - 1];
                index--;
            }
            (*Arr)[Start] = value;

            // Update all the pointers
            Start++;
            Mid++;
            Start2++;
        }
    }
}

template<typename T>
void MergeSortInPlace(array<T>* Arr, int Offset, int Size)
{
    if (Offset < Size) {
        int Divider = Offset + (Size - Offset) / 2;

        MergeSortInPlace(Arr, Offset, Divider);
        MergeSortInPlace(Arr, Divider + 1, Size);

        MergeArrayInPlace(Arr, Offset, Divider, Size);
    }
}

template<typename T>
void MergeSortInPlace(array<T>* Arr)
{
	MergeSortInPlace(Arr, 0, Arr->Size - 1);
}

template<typename T>
void MergeArrayPointerInPlace(array<T>* Arr, int Start, int Mid, int End)
{
    int Start2 = Mid + 1;

    // If the direct merge is already sorted
    if (*((*Arr)[Mid]) <= *((*Arr)[Start2])) {
        return;
    }

    while (Start <= Mid && Start2 <= End) {

        // If element 1 is in right place
        if (*((*Arr)[Start]) <= *((*Arr)[Start2])) {
            Start++;
        }
        else {
            T value = (*Arr)[Start2];
            int index = Start2;

            // Shift all the elements between element 1
            // element 2, right by 1.
            while (index != Start) {
                (*Arr)[index] = (*Arr)[index - 1];
                index--;
            }
            (*Arr)[Start] = value;

            // Update all the pointers
            Start++;
            Mid++;
            Start2++;
        }
    }
}

template<typename T>
void MergeSortPointerInPlace(array<T>* Arr, int Offset, int Size)
{
    if (Offset < Size) {
        int Divider = Offset + (Size - Offset) / 2;

        MergeSortPointerInPlace(Arr, Offset, Divider);
        MergeSortPointerInPlace(Arr, Divider + 1, Size);

        MergeArrayPointerInPlace(Arr, Offset, Divider, Size);
    }
}

template<typename T>
void MergeSortPointerInPlace(array<T>* Arr)
{
	MergeSortPointerInPlace(Arr, 0, Arr->Size - 1);
}

template<typename T>
void Mergearrays(array<T>& Arr1, array<T>& Arr2)
{
	array<T> Arr3 = ArrayNew<T>(Arr1.Size + Arr2.Size);
	Arr3.Size = Arr1.Size + Arr2.Size;

    int i = 0, j = 0, k = 0;

    // Traverse both array
    while (i < Arr1.Size && j < Arr2.Size)
    {
        // Check if current element of first
        // array is smaller than current element
        // of second array. If yes, store first
        // array element and increment first array
        // index. Otherwise do same with second array
        if (Arr1[i] < Arr2[j]){
            Arr3[k++] = Arr1[i++];
        } else {
            Arr3[k++] = Arr2[j++];
		}
	}

    // Store remaining elements of first array
    while (i < Arr1.Size){
        Arr3[k++] = Arr1[i++];
	}

    // Store remaining elements of second array
    while (j < Arr2.Size){
        Arr3[k++] = Arr2[j++];
	}

	ArrayFree(&Arr1);
	Arr1 = Arr3;
}

#ifdef DEBUG
#pragma optimize("g", off)
#endif
