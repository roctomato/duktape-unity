
#include "duk_internal.h"

#include "fix16.h"

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_from_int(duk_context *ctx) {
    duk_push_int(ctx, fix16_from_int(duk_to_int(ctx, 0)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_from_number(duk_context *ctx) {
    duk_push_number(ctx, fix16_from_dbl(duk_to_number(ctx, 0)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_to_int(duk_context *ctx) {
    duk_push_int(ctx, fix16_to_int(duk_to_int(ctx, 0)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_to_number(duk_context *ctx) {
    duk_push_number(ctx, fix16_to_dbl(duk_to_int(ctx, 0)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_add(duk_context *ctx) {
    duk_push_int(ctx, fix16_add(duk_to_int(ctx, 0), duk_to_int(ctx, 1)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_sadd(duk_context *ctx) {
    duk_push_int(ctx, fix16_sadd(duk_to_int(ctx, 0), duk_to_int(ctx, 1)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_sub(duk_context *ctx) {
    duk_push_int(ctx, fix16_sub(duk_to_int(ctx, 0), duk_to_int(ctx, 1)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_ssub(duk_context *ctx) {
    duk_push_int(ctx, fix16_ssub(duk_to_int(ctx, 0), duk_to_int(ctx, 1)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_mul(duk_context *ctx) {
    duk_push_int(ctx, fix16_mul(duk_to_int(ctx, 0), duk_to_int(ctx, 1)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_smul(duk_context *ctx) {
    duk_push_int(ctx, fix16_smul(duk_to_int(ctx, 0), duk_to_int(ctx, 1)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_div(duk_context *ctx) {
    duk_push_int(ctx, fix16_div(duk_to_int(ctx, 0), duk_to_int(ctx, 1)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_sdiv(duk_context *ctx) {
    duk_push_int(ctx, fix16_sdiv(duk_to_int(ctx, 0), duk_to_int(ctx, 1)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_mod(duk_context *ctx) {
    duk_push_int(ctx, fix16_mod(duk_to_int(ctx, 0), duk_to_int(ctx, 1)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_abs(duk_context *ctx) {
    duk_push_int(ctx, fix16_abs(duk_to_int(ctx, 0)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_floor(duk_context *ctx) {
    duk_push_int(ctx, fix16_floor(duk_to_int(ctx, 0)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_ceil(duk_context *ctx) {
    duk_push_int(ctx, fix16_ceil(duk_to_int(ctx, 0)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_min(duk_context *ctx) {
    duk_push_int(ctx, fix16_min(duk_to_int(ctx, 0), duk_to_int(ctx, 1)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_max(duk_context *ctx) {
    duk_push_int(ctx, fix16_max(duk_to_int(ctx, 0), duk_to_int(ctx, 1)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_clamp(duk_context *ctx) {
    duk_push_int(ctx, fix16_clamp(duk_to_int(ctx, 0), duk_to_int(ctx, 1), duk_to_int(ctx, 2)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_clamp01(duk_context *ctx) {
    duk_push_int(ctx, fix16_clamp(duk_to_int(ctx, 0), 0, 0x00010000));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_sqrt(duk_context *ctx) {
    duk_push_int(ctx, fix16_sqrt(duk_to_int(ctx, 0)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_lerp(duk_context *ctx) {
    duk_int_t x = duk_to_int(ctx, 0);
    duk_int_t y = duk_to_int(ctx, 1);
    duk_int_t t = duk_to_int(ctx, 2);
    duk_push_int(ctx, fix16_add(x, fix16_mul(fix16_sub(y, x), t)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_rad2deg(duk_context *ctx) {
    duk_push_int(ctx, fix16_mul(duk_to_int(ctx, 0), 3754936));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_deg2rad(duk_context *ctx) {
    duk_push_int(ctx, fix16_mul(duk_to_int(ctx, 0), 1144));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_sin(duk_context *ctx) {
    duk_push_int(ctx, fix16_sin(duk_to_int(ctx, 0)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_cos(duk_context *ctx) {
    duk_push_int(ctx, fix16_cos(duk_to_int(ctx, 0)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_tan(duk_context *ctx) {
    duk_push_int(ctx, fix16_tan(duk_to_int(ctx, 0)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_asin(duk_context *ctx) {
    duk_push_int(ctx, fix16_asin(duk_to_int(ctx, 0)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_acos(duk_context *ctx) {
    duk_push_int(ctx, fix16_acos(duk_to_int(ctx, 0)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_atan(duk_context *ctx) {
    duk_push_int(ctx, fix16_atan(duk_to_int(ctx, 0)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_atan2(duk_context *ctx) {
    duk_push_int(ctx, fix16_atan2(duk_to_int(ctx, 0), duk_to_int(ctx, 1)));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_sq(duk_context *ctx) {
    duk_int_t x = duk_to_int(ctx, 0);
    duk_push_int(ctx, fix16_mul(x, x));
    return 1;
}

DUK_LOCAL DUK_INLINE duk_ret_t duk_fmath_magnitude(duk_context *ctx) {
    duk_int_t x = duk_to_int(ctx, 0);
    duk_int_t y = duk_to_int(ctx, 1);
    duk_push_int(ctx, fix16_sqrt(fix16_add(fix16_mul(x, x), fix16_mul(y, y))));
    return 1;
}

DUK_INTERNAL duk_bool_t duk_fmath_open(duk_context *ctx) {
    duk_push_global_object(ctx);
    duk_unity_get_prop_object(ctx, -1, "FMath");
    
    duk_unity_add_member(ctx, "to_int", duk_fmath_to_int, -1);
    duk_unity_add_member(ctx, "to_number", duk_fmath_to_number, -1);
    duk_unity_add_member(ctx, "from_int", duk_fmath_from_int, -1);
    duk_unity_add_member(ctx, "from_number", duk_fmath_from_number, -1);

    duk_unity_add_member(ctx, "add", duk_fmath_add, -1);
    duk_unity_add_member(ctx, "sadd", duk_fmath_sadd, -1);
    duk_unity_add_member(ctx, "sub", duk_fmath_sub, -1);
    duk_unity_add_member(ctx, "ssub", duk_fmath_ssub, -1);
    duk_unity_add_member(ctx, "div", duk_fmath_div, -1);
    duk_unity_add_member(ctx, "sdiv", duk_fmath_sdiv, -1);
    duk_unity_add_member(ctx, "mul", duk_fmath_mul, -1);
    duk_unity_add_member(ctx, "smul", duk_fmath_smul, -1);
    duk_unity_add_member(ctx, "mod", duk_fmath_mod, -1);
    duk_unity_add_member(ctx, "abs", duk_fmath_abs, -1);
    duk_unity_add_member(ctx, "floor", duk_fmath_floor, -1);
    duk_unity_add_member(ctx, "ceil", duk_fmath_ceil, -1);
    duk_unity_add_member(ctx, "min", duk_fmath_min, -1);
    duk_unity_add_member(ctx, "max", duk_fmath_max, -1);
    duk_unity_add_member(ctx, "clamp", duk_fmath_clamp, -1);
    duk_unity_add_member(ctx, "clamp01", duk_fmath_clamp01, -1);

    duk_unity_add_member(ctx, "sq", duk_fmath_sq, -1);
    duk_unity_add_member(ctx, "sqrt", duk_fmath_sqrt, -1);
    duk_unity_add_member(ctx, "magnitude", duk_fmath_magnitude, -1);

    duk_unity_add_member(ctx, "lerp", duk_fmath_lerp, -1);

    duk_unity_add_member(ctx, "rad2deg", duk_fmath_rad2deg, -1);
    duk_unity_add_member(ctx, "deg2rad", duk_fmath_deg2rad, -1);
    duk_unity_add_member(ctx, "sin", duk_fmath_sin, -1);
    duk_unity_add_member(ctx, "cos", duk_fmath_cos, -1);
    duk_unity_add_member(ctx, "tan", duk_fmath_tan, -1);
    duk_unity_add_member(ctx, "asin", duk_fmath_asin, -1);
    duk_unity_add_member(ctx, "acos", duk_fmath_acos, -1);
    duk_unity_add_member(ctx, "atan", duk_fmath_atan, -1);
    duk_unity_add_member(ctx, "atan2", duk_fmath_atan2, -1);

    duk_pop_2(ctx); // pop DuktapeJS and global    
    return 1;
}
