struct string {
	char * Data;
	s32 Size;
	
	string() = default;
	string(s8 * Text) { Size = strlen((const char*)Text); Data = (char*)Text; }
	string(char* Text, s32 Len) { Size = Len; Data = Text; }
	string(const char * Text) { Size = strlen(Text); Data = (char*)Text; }
	string(char Char) { Size = 1; Data = &Char; }
	
	operator char *(){ return (char*)Data; }
	operator const char *(){ return (const char*)Data;}
	
	char& operator [](s32 Index) {return Data[Index];}
};

bool operator == (const string x, const string y) {
	if(x.Size != y.Size) return false;
	return strncmp(x.Data, y.Data, x.Size) == 0;
}

//printsl_custom(string S){ printsl("%", (const char *)S.Data);}
printsl_custom(string S){ for(int i = 0; i < S.Size; i++){ printsl("%", S[i]);}}

void StringFree(string* String){
	MemFree(String->Data);
	String->Size = 0;
	String->Data = 0;
}

string StringAlloc(const char * String){
	string Str = {};
	Str.Size = strlen(String);
	Str.Data = (char*)calloc(1, (Str.Size + 1) * sizeof(u8));
	Str.Data[Str.Size + 1] = 0;
	memcpy(Str.Data, String, Str.Size * sizeof(u8));
	return Str;
}

string StringAlloc(const char * String, memory_arena* Arena){
	string Str = {};
	Str.Size = strlen(String);
	Str.Data = (char*)ArenaBump(Arena, (Str.Size + 1) * sizeof(u8));
	memset(Str.Data + Str.Size, 0, sizeof(u8));
	memcpy(Str.Data, String, Str.Size * sizeof(u8));
	return Str;
}

string StringAlloc(const char * String, u32 Size){
	string Str = {};
	Str.Size = Size;
	Str.Data = (char*)calloc(1, Str.Size * sizeof(u8));
	memcpy(Str.Data, String, Str.Size * sizeof(u8));
	return Str;
}

void StringRealloc(string* String,const char * Text){
	if(String->Data){
		StringFree(String);
	}
	*String = StringAlloc(Text);
}

string StringCopy(string String){
	string Str = {};
	Str.Size = String.Size;
	Str.Data = (char*)calloc(1, (String.Size + 1) * sizeof(char));
	memcpy(Str.Data, String.Data, Str.Size * sizeof(char));
	return Str;
}

string StringConcat(string String1, string  String2){
	string Str = {};
	Str.Size = String1.Size + String2.Size;
	Str.Data = (char*)calloc(1, (String1.Size + String2.Size + 1) * sizeof(char));
	memcpy(Str.Data, String1.Data, String1.Size * sizeof(char));
	memcpy(Str.Data + String1.Size, String2.Data, String2.Size * sizeof(char));
	return Str;
}

template<typename T, typename ... args>
string StringFormat(string Text,T T1, args... Args){
	accumulate_into_buffer(Text, T1, Args...);
	string String = StringAlloc(__print_buff,  __buffer_index);
    __buffer_index = 0;
	return String;
}


string operator += (string& x, const string y) {
	string Ret = StringConcat(x, y);
	StringFree(&x);
	x = Ret;
	return x;
}
