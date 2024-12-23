/* date = September 1st 2024 3:54 pm */

#ifndef ASSERT_H
#define ASSERT_H

#ifdef DEBUG
#define Assert(expr, message, ...)  \
if (!(expr)) { \
print("### Assertion failed: '%'", #expr); \
print_tab_if_there_is_a_message(message); \
print(message, ##__VA_ARGS__);  \
print("    File: %", __FILE__); \
print("    Line: %", __LINE__); \
print("    Function: %", __FUNCTION__); \
while(true){}\
}
#define AssertBounds(var, min_val, max_val) Assert(((var) >= (min_val)) && ((var) < (max_val)), "OUT OF BOUNDS! expected between % and % but was %", (min_val), (max_val - 1), (var))
#else
#define Assert(expr, message, ...)
#define AssertBounds(var, min_val, max_val)
#endif

#endif //ASSERT_H
