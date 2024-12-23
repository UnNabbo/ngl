#if 1
#define Log(fmt, ...)   print(fmt, __VA_ARGS__)
#define Logsl(fmt, ...) printsl(fmt, __VA_ARGS__)

#define LogInfo(fmt, ...)     do{ Logsl("\033[92m[INFO]\033[0m ");   Log(fmt, __VA_ARGS__); } while(0)
#define LogTodo(fmt, ...)     do{ Logsl("\033[94m[TODO]\033[0m ");   Log(fmt, __VA_ARGS__); } while(0)
#define LogWarn(fmt, ...)     do{ Logsl("\033[33m[WARN]\033[0m ");   Log(fmt, __VA_ARGS__); } while(0)

#define LogInfosl(fmt, ...)   do{ Logsl("\033[92m[INFO]\033[0m ");   Logsl(fmt, __VA_ARGS__); } while(0)
#define LogTodosl(fmt, ...)   do{ Logsl("\033[94m[TODO]\033[0m ");   Logsl(fmt, __VA_ARGS__); } while(0)
#define LogWarnsl(fmt, ...)   do{ Logsl("\033[33m[WARN]\033[0m ");   Logsl(fmt, __VA_ARGS__); } while(0)

#else

#define Log(fmt, ...)
#define Logsl(fmt, ...)

#define LogInfo(fmt, ...)
#define LogWarn(fmt, ...)

#define LogInfosl(fmt, ...)
#define LogWarnsl(fmt, ...)

#define LogError(cat, sub, spec, fmt, ...)


#endif
