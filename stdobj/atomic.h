
class Atomic {
	public:
		static long Increment(long* addend);
		static long Decrement(long* addend);
		static long Exchange(long* target, long value);
		static long ExchangeAdd(long* addend, long value);
		static void* CompareExchange(void** destination, void* exchange, void* comperand);

};

#ifdef WIN32
long Atomic::Increment(long* addend) { return ::InterlockedIncrement(addend); }
long Atomic::Decrement(long* addend) { return ::InterlockedDecrement(addend); }
long Atomic::Exchange(long* target, long value) { return ::InterlockedExchange(target, value); }
long Atomic::ExchangeAdd(long* addend, long value) {return ::InterlockedExchangeAdd(addend, value); }
void* Atomic::CompareExchange(void** destination, void* exchange, void* comperand)
{ return ::InterlockedCompareExchange(destination, exchange, comperand); }
#endif
