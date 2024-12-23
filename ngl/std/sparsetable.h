/* date = July 5th 2024 2:22 am */

#ifndef SPARSETABLE_H
#define SPARSETABLE_H

template<typename T>
struct sparse_group{
	static_array<T> Table;
	sparse_group * Next;
}

template<typename T>
struct sparse_table{
	sparse_group<T> * Groups;
	u64 GroupCount;
	u64 GroupSize;
	
	T& operator[](s32 Index){
		s32 GroupIndex = (s32)(Index / this->GroupSize);
		Assert(GroupIndex <= GroupCount, "");
		
		sparse_group<T> * Group = this->Groups;
		for(int i = 0; i < GroupIndex; i++){
			if(Group->Next){
				Group = Group->Next;
			}
		}
		
		return Group->Table[Index % GroupSize];
	}
}

#endif //SPARSETABLE_H
