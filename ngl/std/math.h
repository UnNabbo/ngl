union __declspec(intrin_type) __declspec(align(16)) __m128 {
	f32 m128_f32[4];
	f64 m128_f64[2];
    s8  m128_i8[16];
    s16 m128_i16[8];
    s32 m128_i32[4];
    s64 m128_i64[2];
    u8  m128_u8[16];
    u16 m128_u16[8];
    u32 m128_u32[4];
    u64 m128_u64[2];
};

extern "C" {
	extern __m128 _mm_set_ss(float);
	extern __m128 _mm_sqrt_ss(__m128);
	extern __m128 _mm_sqrt_ps(__m128);
	extern f32 _mm_cvtss_f32(__m128);
	extern __m128 _mm_add_ps(__m128, __m128);
	extern __m128 _mm_sub_ps(__m128, __m128);
	extern __m128 _mm_set_ps1(f32);
	extern __m128 _mm_mul_ps(__m128, __m128);
	extern __m128 _mm_div_ps(__m128, __m128);
	extern __m128 _mm_cmpeq_ps(__m128, __m128);
	extern s32 _mm_movemask_ps(__m128);
	extern __m128 _mm_dp_ps(__m128, __m128, s32);
	extern __m128 _mm_setr_ps(f32, f32, f32, f32);
	extern __m128 _mm_or_ps(__m128, __m128);
	
	extern f64 exp(f64);  // Todo - remove this
}

#define _mm_set1_ps(x) _mm_set_ps1(x)

inline u8 count_digits(u64 x){
    u8 n = 0;
    do {
        n++;
    } while(x /= 10);
    return n;
}

inline float npow(float x, u32 n){
    float res = x;
    for(u32 i = 1; i < n; i++){
        res *= x;
    }
    return res;
}
inline float floor(float x)          { return x >= 0 ? int(x) : int(x - 0.9999999999f); }
inline float ceil(float x)           { return x < 0 ? int(x) : int(x + 1); }
inline float round(float x)          { return x >= 0 ? floor(x + 0.5) : ceil(x - 0.5); }
inline float roundn(float x, u32 n)  { return round(x * npow(10, n)) / npow(10, n); }
inline float trunc(float x)          { return float(int(x)); }
inline float remap(float in, float old_from, float old_to, float new_from, float new_to) {
    return (in - old_from) / (old_to - old_from) * (new_to - new_from) + new_from;
}

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) < (b)) ? (b) : (a))
#define abs(a) (((a) >= 0) ? (a) : (-(a)))
#define sgn(a) (((a) == 0) ? (0) : (((a) > 0) ? 1 : -1))
#define clamp(val, min_, max_) (max(min((val), (max_)), (min_)))
#define fmod(x, y) ((x) - trunc((x) / (y)) * (y))
#define lerp(start, dest, t) (((dest) - (start)) * (t) + (start))

#define PI        (3.1415926535f)
#define TAU       (6.2831853072f)
#define E         (2.7182818284f)
#define SQRT2     (1.4142135623f)
#define SQRT3     (1.7320508075f)
#define DEG2RAD   (PI / 180.0f)
#define DEG2TURNS (1.0f / 360.0f)
#define RAD2DEG   (180.0f / PI)
#define RAD2TURNS (1.0f / TAU)
#define TURNS2DEG (360.0f)
#define TURNS2RAD (TAU)

inline float _sin_internal(float x) {
    float q = 8 * x - 16 * x * x;
    return 0.225 * (q * q - q) + q;
}

// trigonometric functions in turns (1 turn = 360 deg)
inline float sin(float angle) {
    angle -= int(angle);
    
    if(angle < 0) angle += 1;
    if(angle > 0.5) return -_sin_internal(angle - 0.5);
    return _sin_internal(angle);
}
// Todo(Quattro): implement other trigonometric functions
inline float cos(float angle)  { return sin(angle + 0.25); }
inline float tan(float angle)  { return sin(angle) / cos(angle); }
inline float cot(float angle)  { return cos(angle) / sin(angle); }
inline float sqrt(float x)     { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(x))); }

struct vec2 {
    union {
        float Ptr[2];
        struct {float x, y;};
    };
    vec2() = default;
    template <typename A, typename B> vec2(A a, B b){
        this->x = (float)a;
        this->y = (float)b;
    }
};

// TODO(Nabbo): Implement all operators:
struct ivec2 {
	union{
        s32 ptr[2];
        struct {s32 x, y;};
    };
	ivec2() = default;
    template <typename A, typename B> ivec2(A a, B b){
        this->x = (s32)a;
        this->y = (s32)b;
    }
	operator vec2(){ return {x, y}; }
};


struct vec3 {
    union {
        float Ptr[3];
        struct {float x, y, z;};
		struct {float r, g, b;};
    };
    vec3() = default;
    template <typename A, typename B, typename C> vec3(A a, B b, C c){
        this->x = (float)a;
        this->y = (float)b;
        this->z = (float)c;
    }
};

struct vec4 {
    union {
        float Ptr[4];
        struct {float x, y, z, w;};
		struct {float r, g, b, a;};
        __m128 V;
    };
    vec4() = default;
    template <typename A, typename B, typename C, typename D> vec4(A a, B b, C c, D d){
        this->x = (float)a;
        this->y = (float)b;
        this->z = (float)c;
        this->w = (float)d;
    }
};

struct mat4 {
    union {
        float Ptr[16];
        float Mat_f[4][4];
        struct {
            float M11, M12, M13, M14;
            float M21, M22, M23, M24;
            float M31, M32, M33, M34;
            float M41, M42, M43, M44;
        };
        struct { __m128 R1, R2, R3, R4; };
        __m128 Mat_r[4];
        struct { vec4 V1, V2, V3, V4; };
        vec4 Mat_v[4];
    };
    inline float* operator [](int idx){
        return this->Mat_f[idx];
    }
};



struct rect{
	union{
		f32 Data[4];
		struct { f32 Left, Top, Right, Bottom; };
		struct { f32 x, y, w, h; };
		__m128 MMX;
	};
};

bool AreRectRegionInterseting(rect A, rect B, rect * Result){
	if(Result){
		f32 Left = max(A.Left, B.Left);
		f32 Right = min(A.Right, B.Right);
		f32 Top = max(A.Top, B.Top);
		f32 Bottom = min(A.Bottom, B.Bottom);
		*Result = rect{Left, Top, Right, Bottom}; 
		return (Right >= Left && Bottom  >= Top);
	}else{
	return A.Left <= B.Right &&
		A.Left <= A.Right &&
		A.Top <= B.Bottom &&
			B.Top <= A.Bottom;
	}
} 

#define QUAD_TO_RECT(Position , Size) rect{Position.x - Size.x * 0.5f, Position.y - Size.y * 0.5f, Position.x + Size.x * 0.5f, Position.y + Size.y * 0.5f}

inline void printsl_custom(rect r)  { printsl("(%, %, %, %)", r.Left, r.Top, r.Right, r.Bottom); }
inline void printsl_custom(vec2 v)  { printsl("(%, %)", v.x, v.y);}
inline void printsl_custom(ivec2 v) { printsl("(%, %)", v.x, v.y);}
inline void printsl_custom(vec3 v)  { printsl("(%, %, %)", v.x, v.y, v.z);}
inline void printsl_custom(vec4 v)  { printsl("(%, %, %, %)", v.x, v.y, v.z, v.w);}
inline void printsl_custom(mat4 m)  { printsl("|% % % %|\n|% % % %|\n|% % % %|\n|% % % %|\n", m.M11, m.M12, m.M13, m.M14, m.M21, m.M22, m.M23, m.M24, m.M31, m.M32, m.M33, m.M34, m.M41, m.M42, m.M43, m.M44);}

inline rect operator +(rect Rect1, const rect Rect2)  {rect Result = {.MMX = _mm_add_ps(Rect1.MMX, Rect2.MMX)};           return Result; }
inline vec2 operator +(vec2 a, vec2 b)  {return {a.x + b.x, a.y + b.y};}
inline ivec2 operator +(ivec2 a, ivec2 b)  {return {a.x + b.x, a.y + b.y};}
inline vec3 operator +(vec3 a, vec3 b)  {return {a.x + b.x, a.y + b.y, a.z + b.z};}
inline vec4 operator +(vec4 a, vec4 b)  {vec4 res; res.V = _mm_add_ps(a.V, b.V); return res;}
inline vec2 operator -(vec2 a, vec2 b)  {return {a.x - b.x, a.y - b.y};}
inline vec3 operator -(vec3 a, vec3 b)  {return {a.x - b.x, a.y - b.y, a.z - b.z};}
inline vec4 operator -(vec4 a, vec4 b)  {vec4 res; res.V = _mm_sub_ps(a.V, b.V); return res;}
inline rect operator *(rect Rect1, const f32 Value)   {rect Result = {.MMX = _mm_mul_ps(Rect1.MMX, _mm_set1_ps(Value)) }; return Result; }
inline vec2 operator *(vec2 a, float s) {return {a.x * s, a.y * s};}
inline vec3 operator *(vec3 a, float s) {return {a.x * s, a.y * s, a.z * s};}
inline vec4 operator *(vec4 a, float s) {vec4 res; res.V = _mm_mul_ps(a.V, _mm_set1_ps(s)); return res;}
inline rect operator *(rect Rect1, const rect Rect2)  {rect Result = {.MMX = _mm_mul_ps(Rect1.MMX, Rect2.MMX)};           return Result; }
inline vec2 operator *(float s, vec2 a) {return {a.x * s, a.y * s};}
inline vec3 operator *(float s, vec3 a) {return {a.x * s, a.y * s, a.z * s};}
inline vec4 operator *(float s, vec4 a) {vec4 res; res.V = _mm_mul_ps(a.V, _mm_set1_ps(s)); return res;}
inline vec2 operator *(vec2 a, vec2 b)  {return {a.x * b.x, a.y * b.y};}
inline vec3 operator *(vec3 a, vec3 b)  {return {a.x * b.x, a.y * b.y, a.z * b.z};}
inline vec4 operator *(vec4 a, vec4 b)  {vec4 res; res.V = _mm_mul_ps(a.V, b.V); return res;}
inline rect operator /(rect Rect1, const f32 Value)   {rect Result = {.MMX = _mm_div_ps(Rect1.MMX, _mm_set1_ps(Value))};   return Result; }
inline vec2 operator /(vec2 a, float s) {return {a.x / s, a.y / s};}
inline vec3 operator /(vec3 a, float s) {return {a.x / s, a.y / s, a.z / s};}
inline vec4 operator /(vec4 a, float s) {vec4 res; res.V = _mm_div_ps(a.V, _mm_set1_ps(s)); return res;}
inline vec2 operator /(float s, vec2 a) {return {s / a.x, s / a.y};}
inline vec3 operator /(float s, vec3 a) {return {s / a.x, s / a.y, s / a.z};}
inline vec4 operator /(float s, vec4 a) {vec4 res; res.V = _mm_div_ps(_mm_set1_ps(s), a.V); return res;}
inline rect operator /(rect Rect1, const rect Rect2)  {rect Result = {.MMX = _mm_div_ps(Rect1.MMX, Rect2.MMX)};           return Result; }
inline vec2 operator /(vec2 a, vec2 b)  {return {a.x / b.x, a.y / b.y};}
inline vec3 operator /(vec3 a, vec3 b)  {return {a.x / b.x, a.y / b.y, a.z / b.z};}
inline vec4 operator /(vec4 a, vec4 b)  {vec4 res; res.V = _mm_div_ps(a.V, b.V); return res;}
inline vec2 operator -(vec2 a)  {return {-a.x, -a.y};}
inline vec3 operator -(vec3 a)  {return {-a.x, -a.y, -a.z};}
inline vec4 operator -(vec4 a)  {vec4 res;  res.V = _mm_sub_ps(_mm_set1_ps(0), a.V);  return res;}
inline rect operator +=(rect& Rect1, const rect Rect2) {Rect1.MMX   = _mm_add_ps(Rect1.MMX, Rect2.MMX); return Rect1; }
inline vec2 operator +=(vec2& a, const vec2 &b) {a = a + b;  return a;}
inline ivec2 operator +=(ivec2& a, const ivec2 &b) {a = a + b;  return a;}
inline vec3 operator +=(vec3& a, const vec3 &b) {a = a + b;  return a;}
inline vec4 operator +=(vec4& a, const vec4 &b) {a = a + b;  return a;}
inline vec2 operator -=(vec2& a, const vec2 &b) {a = a - b;  return a;}
inline vec3 operator -=(vec3& a, const vec3 &b) {a = a - b;  return a;}
inline vec4 operator -=(vec4& a, const vec4 &b) {a = a - b;  return a;}
inline vec2 operator *=(vec2& a, const float &b) {a = a * b;  return a;}
inline vec3 operator *=(vec3& a, const float &b) {a = a * b;  return a;}
inline vec4 operator *=(vec4& a, const float &b) {a = a * b;  return a;}
inline vec2 operator /=(vec2& a, const float &b) {a = a / b;  return a;}
inline vec3 operator /=(vec3& a, const float &b) {a = a / b;  return a;}
inline vec4 operator /=(vec4& a, const float &b) {a = a / b;  return a;}
inline bool operator ==(vec2 a, vec2 b) {return (a.x == b.x) && (a.y == b.y);}
inline bool operator ==(vec3 a, vec3 b) {return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);}
inline bool operator ==(vec4 a, vec4 b) {return _mm_movemask_ps(_mm_cmpeq_ps(a.V, b.V)) == 0b1111;}
inline bool operator !=(vec2 a, vec2 b) {return !((a.x == b.x) && (a.y == b.y));}
inline bool operator !=(vec3 a, vec3 b) {return !((a.x == b.x) && (a.y == b.y) && (a.z == b.z));}
inline bool operator !=(vec4 a, vec4 b) {return _mm_movemask_ps(_mm_cmpeq_ps(a.V, b.V)) != 0b1111;}


inline vec2 vec2_round(vec2 v) {return {round(v.x), round(v.y)};}
inline vec3 vec3_round(vec3 v) {return {round(v.x), round(v.y), round(v.z)};}
inline vec4 vec4_round(vec4 v) {return {round(v.x), round(v.y), round(v.z), round(v.w)};}

inline vec2 vec2_floor(vec2 v) {return {floor(v.x), floor(v.y)};}
inline vec3 vec3_floor(vec3 v) {return {floor(v.x), floor(v.y), floor(v.z)};}
inline vec4 vec4_floor(vec4 v) {return {floor(v.x), floor(v.y), floor(v.z), floor(v.w)};}

inline vec2 vec2_ceil(vec2 v)  {return {ceil(v.x), ceil(v.y)};}
inline vec3 vec3_ceil(vec3 v)  {return {ceil(v.x), ceil(v.y), ceil(v.z)};}
inline vec4 vec4_ceil(vec4 v)  {return {ceil(v.x), ceil(v.y), ceil(v.z), ceil(v.w)};}

inline vec2 vec2_trunc(vec2 v) {return {trunc(v.x), trunc(v.y)};}
inline vec3 vec3_trunc(vec3 v) {return {trunc(v.x), trunc(v.y), trunc(v.z)};}
inline vec4 vec4_trunc(vec4 v) {return {trunc(v.x), trunc(v.y), trunc(v.z), trunc(v.w)};}

inline vec2 remap(vec2 in, vec2 old_from, vec2 old_to, vec2 new_from, vec2 new_to) {
    return vec2{
        remap(in.x, old_from.x, old_to.x, new_from.x, new_to.x),
        remap(in.y, old_from.y, old_to.y, new_from.y, new_to.y)
    };
}

inline vec3 remap(vec3 in, vec3 old_from, vec3 old_to, vec3 new_from, vec3 new_to) {
    return vec3{
        remap(in.x, old_from.x, old_to.x, new_from.x, new_to.x),
        remap(in.y, old_from.y, old_to.y, new_from.y, new_to.y),
        remap(in.z, old_from.z, old_to.z, new_from.z, new_to.z)
    };
}

inline vec4 remap(vec4 in, vec4 old_from, vec4 old_to, vec4 new_from, vec4 new_to) {
    vec4 num;
    num.V = _mm_sub_ps(in.V, old_from.V);
    vec4 den;
    den.V = _mm_sub_ps(old_to.V, old_from.V);
    vec4 t;
    t.V = _mm_sub_ps(new_to.V, new_from.V);
    vec4 res;
    res.V = _mm_div_ps(num.V, den.V);
    res.V = _mm_mul_ps(res.V, t.V);
    res.V = _mm_add_ps(res.V, new_from.V);
    return res;
}


inline float vec2_length(vec2 v) { return sqrt(v.x * v.x + v.y * v.y); }
inline float vec3_length(vec3 v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }
inline float vec4_length(vec4 v) {
    vec4 res;
    res.V = _mm_dp_ps(v.V, v.V, 0b11110001);
    res.V = _mm_sqrt_ss(res.V);
    return _mm_cvtss_f32(res.V);
}

inline vec2 vec2_normalize(vec2 v){
    float len = vec2_length(v);
    return {v.x / len, v.y / len};
}
inline vec3 vec3_normalize(vec3 v){
    float len = vec3_length(v);
    return {v.x / len, v.y / len, v.z / len};
}
inline vec4 vec4_normalize(vec4 v){
    vec4 res;
    res.V = _mm_div_ps(v.V, _mm_sqrt_ps(_mm_dp_ps(v.V, v.V, 0b11111111)));
    return res;
}

inline float vec2_dot(vec2 a, vec2 b){ return a.x * b.x + a.y * b.y; }
inline float vec3_dot(vec3 a, vec3 b){ return a.x * b.x + a.y * b.y + a.z * b.z; }
inline float vec4_dot(vec4 a, vec4 b){ return _mm_cvtss_f32(_mm_dp_ps(a.V, b.V, 0b11110001)); }


inline vec3 vec3_cross(vec3 a, vec3 b){
    vec3 res;
    res.x = a.y * b.z - b.y * a.z;
    res.y = b.x * a.z - a.x * b.z;
    res.z = a.x * b.y - b.x * a.y;
    return res;
}

inline vec2 vec2_rotate(vec2 v, float angle){
    float cos_value = cos(angle);
    float sin_value = sin(angle);
    
    vec2 res;
    res.x = v.x * cos_value - v.y * sin_value;
    res.y = v.x * sin_value + v.y * cos_value;
    return res;
}
inline vec2 vec2_project_point_on_line(vec2 point, vec2 line_start, vec2 line_dir) {
    // API(cogno): can we make it faster with 2D PGA?
    
    auto dot_value = line_dir.x * (point.x - line_start.x) + line_dir.y * (point.y - line_start.y);
    vec2 proj = line_start + line_dir * dot_value;
    return proj;
}


inline mat4 mat4_new(float n){
    mat4 res;
    res.R1 = _mm_setr_ps(n, 0, 0, 0);
    res.R2 = _mm_setr_ps(0, n, 0, 0);
    res.R3 = _mm_setr_ps(0, 0, n, 0);
    res.R4 = _mm_setr_ps(0, 0, 0, n);
    return res;
}
inline mat4 mat4_transpose(mat4 m) {
    mat4 res;
    res.R1 = _mm_setr_ps(m.M11, m.M21, m.M31, m.M41);
    res.R2 = _mm_setr_ps(m.M12, m.M22, m.M32, m.M42);
    res.R3 = _mm_setr_ps(m.M13, m.M23, m.M33, m.M43);
    res.R4 = _mm_setr_ps(m.M14, m.M24, m.M34, m.M44);
    return res;
}
inline mat4 operator +(mat4 m1, mat4 m2){
    mat4 res;
    res.R1 = _mm_add_ps(m1.R1, m2.R1);
    res.R2 = _mm_add_ps(m1.R2, m2.R2);
    res.R3 = _mm_add_ps(m1.R3, m2.R3);
    res.R4 = _mm_add_ps(m1.R4, m2.R4);
    return res;
}
inline mat4 operator -(mat4 m1, mat4 m2){
    mat4 res;
    res.R1 = _mm_sub_ps(m1.R1, m2.R1);
    res.R2 = _mm_sub_ps(m1.R2, m2.R2);
    res.R3 = _mm_sub_ps(m1.R3, m2.R3);
    res.R4 = _mm_sub_ps(m1.R4, m2.R4);
    return res;
}
inline mat4 operator *(mat4 m, float s){
    mat4 res;
    __m128 val = _mm_set1_ps(s);
    res.R1 = _mm_mul_ps(m.R1, val);
    res.R2 = _mm_mul_ps(m.R2, val);
    res.R3 = _mm_mul_ps(m.R3, val);
    res.R4 = _mm_mul_ps(m.R4, val);
    return res;
}
inline mat4 operator *(float s, mat4 m){
    mat4 res;
    __m128 val = _mm_set1_ps(s);
    res.R1 = _mm_mul_ps(m.R1, val);
    res.R2 = _mm_mul_ps(m.R2, val);
    res.R3 = _mm_mul_ps(m.R3, val);
    res.R4 = _mm_mul_ps(m.R4, val);
    return res;
}
inline mat4 operator /(mat4 m1, float s){
    mat4 res;
    __m128 val = _mm_set1_ps(s);
    res.R1 = _mm_div_ps(m1.R1, val);
    res.R2 = _mm_div_ps(m1.R2, val);
    res.R3 = _mm_div_ps(m1.R3, val);
    res.R4 = _mm_div_ps(m1.R4, val);
    return res;
}
inline mat4 operator /(float s, mat4 m1){
    mat4 res;
    __m128 val = _mm_set1_ps(s);
    res.R1 = _mm_div_ps(val, m1.R1);
    res.R2 = _mm_div_ps(val, m1.R2);
    res.R3 = _mm_div_ps(val, m1.R3);
    res.R4 = _mm_div_ps(val, m1.R4);
    return res;
}
inline mat4 operator *(mat4 m1, mat4 m2){
    mat4 temp = mat4_transpose(m2);
    
    mat4 res;
    res.R1 = _mm_or_ps(_mm_or_ps(_mm_dp_ps(m1.R1, temp.R1, 0b11110001), _mm_dp_ps(m1.R1, temp.R2, 0b11110010)), _mm_or_ps(_mm_dp_ps(m1.R1, temp.R3, 0b11110100), _mm_dp_ps(m1.R1, temp.R4, 0b11111000)));
    res.R2 = _mm_or_ps(_mm_or_ps(_mm_dp_ps(m1.R2, temp.R1, 0b11110001), _mm_dp_ps(m1.R2, temp.R2, 0b11110010)), _mm_or_ps(_mm_dp_ps(m1.R2, temp.R3, 0b11110100), _mm_dp_ps(m1.R2, temp.R4, 0b11111000)));
    res.R3 = _mm_or_ps(_mm_or_ps(_mm_dp_ps(m1.R3, temp.R1, 0b11110001), _mm_dp_ps(m1.R3, temp.R2, 0b11110010)), _mm_or_ps(_mm_dp_ps(m1.R3, temp.R3, 0b11110100), _mm_dp_ps(m1.R3, temp.R4, 0b11111000)));
    res.R4 = _mm_or_ps(_mm_or_ps(_mm_dp_ps(m1.R4, temp.R1, 0b11110001), _mm_dp_ps(m1.R4, temp.R2, 0b11110010)), _mm_or_ps(_mm_dp_ps(m1.R4, temp.R3, 0b11110100), _mm_dp_ps(m1.R4, temp.R4, 0b11111000)));
    return res;
}
inline vec4 operator* (mat4 m, vec4 v) {
    vec4 res;
    res.V = _mm_or_ps(_mm_or_ps(_mm_dp_ps(m.R1, v.V, 0b11110001), _mm_dp_ps(m.R2, v.V, 0b11110010)), _mm_or_ps(_mm_dp_ps(m.R3, v.V, 0b11110100), _mm_dp_ps(m.R4, v.V, 0b11111000)));
    return res;
}
inline vec4 operator* (vec4 v, mat4 m) {
    vec4 res;
    mat4 mt = mat4_transpose(m);
    res.V = _mm_or_ps(_mm_or_ps(_mm_dp_ps(mt.R1, v.V, 0b11110001), _mm_dp_ps(mt.R2, v.V, 0b11110010)), _mm_or_ps(_mm_dp_ps(mt.R3, v.V, 0b11110100), _mm_dp_ps(mt.R4, v.V, 0b11111000)));
    return res;
}
inline mat4 operator +=(mat4& m1, const mat4& m2) { m1 = m1 + m2; return m1;}
inline mat4 operator -=(mat4& m1, const mat4& m2) { m1 = m1 - m2; return m1;}
inline mat4 operator *=(mat4& m1, const float& b) { m1 = m1 * b; return m1; }
inline mat4 operator *=(mat4& m1, const mat4& m2) { m1 = m1 * m2; return m2; }
inline mat4 operator /=(mat4& m1, const float& b) { m1 = m1 / b; return m1; }
inline mat4 perspective(float fov, float aspect_ratio, float z_near, float z_far){
    mat4 res = {};
    float tan_value = tan(fov / 2.0f);
    float cotangent = 1.0f / tan_value;
    res.M11 = 1 / (aspect_ratio * tan_value);
    res.M22 = cotangent;
    res.M33 = - (z_far + z_near) / (z_far - z_near);
    res.M34 = -1.0;
    res.M43 = -(2.0 * z_near * z_far) / (z_far - z_near);
    return res;
}
inline mat4 ortho(float left, float right, float bottom, float top, float z_near, float z_far){
    mat4 res = {};
    res.M11 = 2.0f / (right - left);
    res.M22 = 2.0f / (top - bottom);
    res.M33 = -2.0f / (z_far - z_near);
    res.M41 = -(right + left) / (right - left);
    res.M42 = -(top + bottom) / (top - bottom);
    res.M43 = -(z_far + z_near) / (z_far - z_near);
    res.M44 = 1;
    return res;
}
inline mat4 ortho(float left, float right, float bottom, float top) {
    mat4 res = ortho(left, right, bottom, top, -2, 0);
    return res;
}
inline float determinant(mat4 m){
    vec3 col1 = {m.M11, m.M21, m.M31};
    vec3 col2 = {m.M12, m.M22, m.M32};
    vec3 col3 = {m.M13, m.M23, m.M33};
    vec3 col4 = {m.M14, m.M24, m.M34};
    
    vec3 C01 = vec3_cross(col1, col2);
    vec3 C23 = vec3_cross(col3, col4);
    vec3 B10 = col1 * m.M42 - col2 * m.M41;
    vec3 B32 = col2 * m.M44 - col4 * m.M43;
    
    return vec3_dot(C01, B32) + vec3_dot(C23, B10);
}
inline mat4 mat4_inverse(mat4 m){
    vec3 col1 = {m.M11, m.M21, m.M31};
    vec3 col2 = {m.M12, m.M22, m.M32};
    vec3 col3 = {m.M13, m.M23, m.M33};
    vec3 col4 = {m.M14, m.M24, m.M34};
    
    vec3 C01 = vec3_cross(col1, col2);
    vec3 C23 = vec3_cross(col3, col4);
    vec3 B10 = col1 * m.M42 - col2 * m.M41;
    vec3 B32 = col3 * m.M44 - col4 * m.M43;
    
    float inv_det = 1.0f / (vec3_dot(C01, B32) + vec3_dot(C23, B10));
    C01 = C01 * inv_det;
    C23 = C23 * inv_det;
    B10 = B10 * inv_det;
    B32 = B32 * inv_det;

    mat4 res;
    vec3 i1 = (vec3_cross(col2, B32) + (C23 * m.M42));
    vec3 i2 = (vec3_cross(B32, col1) - (C23 * m.M41));
    vec3 i3 = (vec3_cross(col4, B10) + (C01 * m.M44));
    vec3 i4 = (vec3_cross(B10, col3) - (C01 * m.M43));
    
    res.R1 = _mm_setr_ps(i1.x, i1.y, i1.z, -vec3_dot(col2, C23));
    res.R2 = _mm_setr_ps(i2.x, i2.y, i2.z, +vec3_dot(col1, C23));
    res.R3 = _mm_setr_ps(i3.x, i3.y, i3.z, -vec3_dot(col4, C01));
    res.R4 = _mm_setr_ps(i4.x, i4.y, i4.z, +vec3_dot(col3, C01));

    return res;
}
inline mat4 mat4_translation_mat(vec3 v){
    mat4 res = mat4_new(1);
    res.M14 = v.x;
    res.M24 = v.y;
    res.M34 = v.z;
    return res;
}
inline mat4 mat4_scale_mat(vec3 v){
    mat4 res = {};
    res.M11 = v.x;
    res.M22 = v.y;
    res.M33 = v.z;
    res.M44 = 1;
    return res;
}
inline mat4 mat4_rotation_x_mat(float angle){
    mat4 res = {};
    float c = cos(angle);
    float s = sin(angle);
    
    res.M11 = 1;
    res.M22 = c;  res.M23 = -s;
    res.M32 = s;  res.M33 = c;
    res.M44 = 1;
    return res;
}
inline mat4 mat4_rotation_y_mat(float angle){
    mat4 res = {};
    float c = cos(angle);
    float s = sin(angle);
    
    res.M11 = c;  res.M13 = s;
    res.M22 = 1;
    res.M31 = -s; res.M33 = c;
    res.M44 = 1;
    return res;
}
inline mat4 mat4_rotation_z_mat(float angle){
    mat4 res = {};
    float c = cos(angle);
    float s = sin(angle);
    
    res.M11 = c;  res.M12 = -s;
    res.M21 = s;  res.M22 = c;
    res.M33 = 1;
    res.M44 = 1;
    return res;
}
inline mat4 mat4_rotation_mat(vec3 axis, float angle){
    mat4 res = {}; 
    float c = cos(angle);
    float s = sin(angle);

    axis = vec3_normalize(axis);
    vec3 temp = axis * (1 - c);
    
    res.M11 = c + temp.x * axis.x;
    res.M12 = temp.x * axis.y + s * axis.z;
    res.M13 = temp.x * axis.z - s * axis.y;

    res.M21 = temp.y * axis.x - s * axis.z;
    res.M22 = c + temp.y * axis.y;
    res.M23 = temp.y * axis.z + s * axis.x;

    res.M31 = temp.z * axis.x + s * axis.y;
    res.M32 = temp.z * axis.y - s * axis.x;
    res.M33 = c + temp.z * axis.z;
    
    res.M44 = 1;
    return res;
}

struct Rotor {
    float S;
    float E12, E31, E23;
};

#define DEFAULT_EPSILON 0.01f

inline Rotor rotor_new(vec3 v1, vec3 v2) {
    vec3 a = vec3_normalize(v1);
    vec3 b = vec3_normalize(v2);
    
    Rotor r;
    r.S = vec3_dot(a, b);
    r.E12 = a.x * b.y - a.y * b.x;
    r.E31 = a.z * b.y - a.y * b.z;
    r.E23 = a.x * b.z - a.z * b.x;
    return r;
}

inline Rotor rotor_new(vec3 rot_axis, float turns) {
    vec3 v = vec3_normalize(rot_axis);
    float c = cos(turns / 2);
    float s = sin(turns / 2);
    
    Rotor r;
    r.S = c;
    r.E12 = v.z * s;
    r.E31 = v.y * s;
    r.E23 = v.x * s;
    return r;
}

inline vec3 vec3_rotate(vec3 to_rotate, Rotor r) {
    vec3 v = to_rotate;
    //r is ab
    
    //first product ((ba) * v)
    float x =   v.x * r.S   - v.y * r.E12 + v.z * r.E31; // e1
    float y =   v.y * r.S   + v.x * r.E12 - v.z * r.E23; // e2
    float z =   v.z * r.S   - v.x * r.E31 + v.y * r.E23; // e3
    float w = - v.y * r.E31 - v.z * r.E12 - v.x * r.E23; // e123
    
    //second product (first * r)
    vec3 out;
    out.x =   x * r.S   - y * r.E12 + z * r.E31 - w * r.E23;
    out.y =   x * r.E12 + y * r.S   - w * r.E31 - z * r.E23;
    out.z = - x * r.E31 + y * r.E23 + z * r.S   - w * r.E12;
    
    float sanity_check = w * r.S + z * r.E12 + y * r.E31 + x * r.E23;
    Assert(abs(sanity_check) <= DEFAULT_EPSILON, "wRONG ROTOR CALCULATIONS, rotor was supposed to be fully 3d but it had a non zero bivector component");
    return out;
}


#define vec2_new(N) vec2{N, N}
#define vec3_new(N) vec3{N, N, N}
#define vec4_new(N) vec4{N, N, N, N}
