extern "C" {
	extern long _InterlockedExchangeAdd_HLEAcquire(long volatile*, long);
	extern long _InterlockedExchangeAdd_HLERelease(long volatile*, long);
	extern __int64 _InterlockedExchangeAdd64(__int64 volatile*, __int64);
	extern __int64 _InterlockedExchange64(__int64 volatile*, __int64);
	extern long _InterlockedExchangeAdd(long volatile*, long);
	extern long _InterlockedIncrement(long volatile*);
	extern long _InterlockedDecrement(long volatile*);
	extern long _InterlockedExchange(long volatile*, long);
	// extern void* _InterlockedExchangePointer(void volatile*, void*);
}

struct atomic_i32{
	s32 Value = 0;
	
	operator s32(){ return _InterlockedExchangeAdd_HLERelease((long *)&Value, 0); }
};

INLINE long AtomicAdd(atomic_i32& A, atomic_i32& B){
	return _InterlockedExchangeAdd((long *)&A.Value, B);
}

INLINE long AtomicAdd(atomic_i32& A, long B){
	return _InterlockedExchangeAdd((long *)&A.Value, B);
}

INLINE void AtomicAddAcq(atomic_i32& A, atomic_i32& B){
	_InterlockedExchangeAdd((long *)&A.Value, B);
}

INLINE void AtomicAddAcq(atomic_i32& A, long B){
	_InterlockedExchangeAdd_HLEAcquire((long *)&A.Value, B);
}

INLINE void AtomicIncrement(atomic_i32& A){
	_InterlockedIncrement((long *)&A.Value);
}

INLINE void AtomicDecrement(atomic_i32& A){
	_InterlockedDecrement((long *)&A.Value);
}

INLINE void AtomicAssign(atomic_i32& A, atomic_i32& B){
	_InterlockedExchange((long *)&A.Value, B);
}

INLINE void AtomicAssign(atomic_i32& A, long B){
	_InterlockedExchange((long *)&A.Value, B);
}


struct atomic_i64{
	s64 Value = 0;
	
	operator s64(){ return _InterlockedExchangeAdd((long *)&Value, 0); }
};


INLINE void AtomicAdd(atomic_i64& A, atomic_i64& B){
	_InterlockedExchangeAdd64((__int64 *)&A.Value, B);
}

INLINE void AtomicAdd(atomic_i64& A, s64 B){
	_InterlockedExchangeAdd64((__int64 *)&A.Value, B);
}


INLINE void AtomicIncrement(atomic_i64& A){
	_InterlockedIncrement((long *)&A.Value);
}

INLINE void AtomicDecrement(atomic_i64& A){
	_InterlockedDecrement((long *)&A.Value);
}

INLINE void AtomicAssign(atomic_i64& A, atomic_i64& B){
	_InterlockedExchange64((__int64 *)&A.Value, B);
}

INLINE void AtomicAssign(atomic_i64& A, __int64 B){
	_InterlockedExchange64((__int64 *)&A.Value, B);
}

template<typename T>
struct atomic_ptr{
	T* Ptr;
	
	operator T*(){ return _InterlockedExchangePointer((void*)Ptr, (void*)Ptr); }
};

#define WAIT_FOR_VALUE(Comparioson) while(!(Comparioson)){}

inline void printsl_custom(atomic_i32 v) { printsl("%", v.Value); }
inline void printsl_custom(atomic_i64 v) { printsl("%", v.Value); }
