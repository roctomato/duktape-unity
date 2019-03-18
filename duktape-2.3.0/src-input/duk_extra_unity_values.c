#include "duk_internal.h"

/**
 * Vector3
 */
#define UNITY_VECTOR3_kEpsilon 0.00001F
#define UNITY_SINGLE_Epsilon 1.401298E-45F
#define UNITY_VECTOR3_kEpsilonNormalSqrt 1e-15F
#define UNITY_VECTOR3_k1OverSqrt2 0.7071067811865475244008443621048490F
#define UNITY_MATH_PI 3.1415926535897932384626433832795028841971693993F

DUK_INTERNAL DUK_INLINE float vec3_magnitude(const float* lhs) {
    return sqrtf(lhs[0] * lhs[0] + lhs[1] * lhs[1] + lhs[2] * lhs[2]);
}

DUK_INTERNAL DUK_INLINE float vec3_dot(const float* lhs, const float* rhs) {
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}

DUK_INTERNAL DUK_INLINE void vec3_normalize(float* lhs) {
    float mag = sqrtf(lhs[0] * lhs[0] + lhs[1] * lhs[1] + lhs[2] * lhs[2]);
    lhs[0] /= mag;
    lhs[1] /= mag;
    lhs[2] /= mag;
}

DUK_INTERNAL DUK_INLINE void vec3_cross(const float* lhs, const float* rhs, float* res) {
    res[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    res[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
    res[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
}

DUK_INTERNAL DUK_INLINE void vec3_multiply(float* out_vec3, float a) {
    out_vec3[0] *= a;
    out_vec3[1] *= a;
    out_vec3[2] *= a;
}

DUK_INTERNAL DUK_INLINE void m3x3_multiply_vec3(float* mat3x3, const float* vec3, float* out_vec3) {
	out_vec3[0] = mat3x3[0] * vec3[0] + mat3x3[3] * vec3[1] + mat3x3[6] * vec3[2];
	out_vec3[1] = mat3x3[1] * vec3[0] + mat3x3[4] * vec3[1] + mat3x3[7] * vec3[2];
	out_vec3[2] = mat3x3[2] * vec3[0] + mat3x3[5] * vec3[1] + mat3x3[8] * vec3[2];
}

DUK_INTERNAL DUK_INLINE void m3x3_set_axis_angle(float* mat3x3, const float* vec, float radians) 
{
    /* This function contributed by Erich Boleyn (erich@uruk.org) */
    /* This function used from the Mesa OpenGL code (matrix.c)  */
    float s, c;
    float vx, vy, vz, xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

    s = sin (radians);
    c = cos (radians);

    vx = vec[0];
    vy = vec[1];
    vz = vec[2];

    xx = vx * vx;
    yy = vy * vy;
    zz = vz * vz;
    xy = vx * vy;
    yz = vy * vz;
    zx = vz * vx;
    xs = vx * s;
    ys = vy * s;
    zs = vz * s;
    one_c = 1.0F - c;

    mat3x3[0*3+0] = (one_c * xx) + c;
    mat3x3[1*3+0] = (one_c * xy) - zs;
    mat3x3[2*3+0] = (one_c * zx) + ys;

    mat3x3[0*3+1] = (one_c * xy) + zs;
    mat3x3[1*3+1] = (one_c * yy) + c;
    mat3x3[2*3+1] = (one_c * yz) - xs;

    mat3x3[0*3+2] = (one_c * zx) - ys;
    mat3x3[1*3+2] = (one_c * yz) + xs;
    mat3x3[2*3+2] = (one_c * zz) + c;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_constructor(duk_context *ctx) {
    float x = (float)duk_get_number_default(ctx, 0, 0.0);
    float y = (float)duk_get_number_default(ctx, 1, 0.0);
    float z = (float)duk_get_number_default(ctx, 2, 0.0);
    duk_push_this(ctx);
    duk_unity_put3f(ctx, -1, x, y, z);
    return 0;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_lerp(duk_context *ctx) {
    float ax, ay, az;
    float bx, by, bz;
    float cx, cy, cz;
    float t;
    duk_unity_get3f(ctx, 0, &ax, &ay, &az);
    duk_unity_get3f(ctx, 1, &bx, &by, &bz);
    t = (float)duk_get_number_default(ctx, 2, 0.0);
    t = t > 1.0f ? 1.0f : (t < 0.0f ? 0.0f : t);
    cx = ax + (bx - ax) * t;
    cy = ay + (by - ay) * t;
    cz = az + (bz - az) * t;
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, cx);
    duk_push_number(ctx, cy);
    duk_push_number(ctx, cz);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_lerpUnclamped(duk_context *ctx) {
    float ax, ay, az;
    float bx, by, bz;
    float cx, cy, cz;
    float t;
    duk_unity_get3f(ctx, 0, &ax, &ay, &az);
    duk_unity_get3f(ctx, 1, &bx, &by, &bz);
    t = (float)duk_get_number_default(ctx, 2, 0.0);
    cx = ax + (bx - ax) * t;
    cy = ay + (by - ay) * t;
    cz = az + (bz - az) * t;
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, cx);
    duk_push_number(ctx, cy);
    duk_push_number(ctx, cz);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_moveTowards(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    float rhsx, rhsy, rhsz;
    float maxDistanceDelta;
    duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
    duk_unity_get3f(ctx, 1, &rhsx, &rhsy, &rhsz);
    maxDistanceDelta = (float)duk_get_number_default(ctx, 2, 0.0);

    float tox, toy, toz;
    tox = rhsx - lhsx;
    toy = rhsy - lhsy;
    toz = rhsz - lhsz;
    float dist = sqrtf(tox * tox + toy * toy + toz * toz);
    if (dist < maxDistanceDelta || dist < UNITY_SINGLE_Epsilon) {
        duk_dup(ctx, 1);
        return 1;
    }
    float dd = 1.0f / dist * maxDistanceDelta;
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, lhsx + tox * dd);
    duk_push_number(ctx, lhsy + toy * dd);
    duk_push_number(ctx, lhsz + toz * dd);
    duk_new(ctx, 3);
    return 1;
}

/*
public static Vector3 SmoothDamp(
    0 Vector3 current, 
    1 Vector3 target,
    2 ref Vector3 currentVelocity,
    3 float smoothTime, 
    4 float maxSpeed,
    5 float deltaTime)
*/
DUK_LOCAL duk_ret_t duk_unity_vector3_static_smoothDamp(duk_context *ctx) {
    float current_x, current_y, current_z;
    duk_unity_get3f(ctx, 0, &current_x, &current_y, &current_z);
    float target_x, target_y, target_z;
    duk_unity_get3f(ctx, 1, &target_x, &target_y, &target_z);
    float currentVelocity_x, currentVelocity_y, currentVelocity_z;
    duk_unity_get3f(ctx, 2, &currentVelocity_x, &currentVelocity_y, &currentVelocity_z);
    float smoothTime = (float)duk_get_number_default(ctx, 3, 0.0001);
    if (smoothTime > 0.0001f) {
        smoothTime = 0.0001f;
    }
    float maxSpeed = (float)duk_get_number_default(ctx, 4, INFINITY);
    float deltaTime = (float)duk_get_number_default(ctx, 5, 0.01667);

    float omega = 2.0f / smoothTime;
    float x = omega * deltaTime;
    float exp_ = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
    float change_x = current_x - target_x;
    float change_y = current_y - target_y;
    float change_z = current_z - target_z;
    float originalTo_x = target_x;
    float originalTo_y = target_y;
    float originalTo_z = target_z;
    float maxChange = maxSpeed * smoothTime;
    float change_sqrmag = change_x * change_x + change_y * change_y + change_z * change_z;
    if (change_sqrmag > maxChange * maxChange) {
        float change_mag = sqrtf(change_sqrmag);
        float change_f = maxChange / change_mag;
        change_x *= change_f;
        change_y *= change_f;
        change_z *= change_f;
    }
    target_x = current_x - change_x;
    target_y = current_y - change_y;
    target_z = current_z - change_z;
    float temp_f = omega * change_x * deltaTime;
    float temp_x = currentVelocity_x * deltaTime + temp_f;
    float temp_y = currentVelocity_y * deltaTime + temp_f;
    float temp_z = currentVelocity_z * deltaTime + temp_f;
    currentVelocity_x = (currentVelocity_x - omega * temp_x) * exp_;
    currentVelocity_y = (currentVelocity_y - omega * temp_y) * exp_;
    currentVelocity_z = (currentVelocity_z - omega * temp_z) * exp_;
    float output_x = target_x + (change_x + temp_x) * exp_;
    float output_y = target_y + (change_y + temp_y) * exp_;
    float output_z = target_z + (change_z + temp_z) * exp_;
    
    float dpa_x = originalTo_x - current_x;
    float dpa_y = originalTo_y - current_y;
    float dpa_z = originalTo_z - current_z;
    float dpb_x = output_x - originalTo_x;
    float dpb_y = output_y - originalTo_y;
    float dpb_z = output_z - originalTo_z;
    if (dpa_x * dpb_x + dpa_y * dpb_y + dpa_z * dpb_z > 0) {
        output_x = originalTo_x;
        output_y = originalTo_y;
        output_z = originalTo_z;
        currentVelocity_x = (output_x - originalTo_x) / deltaTime;
        currentVelocity_y = (output_y - originalTo_y) / deltaTime;
        currentVelocity_z = (output_z - originalTo_z) / deltaTime;
    }
    duk_push_number(ctx, currentVelocity_x);
    duk_put_prop_index(ctx, 2, 0);
    duk_push_number(ctx, currentVelocity_y);
    duk_put_prop_index(ctx, 2, 1);
    duk_push_number(ctx, currentVelocity_z);
    duk_put_prop_index(ctx, 2, 2);
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, output_x);
    duk_push_number(ctx, output_y);
    duk_push_number(ctx, output_z);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_Add(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    float rhsx, rhsy, rhsz;
    duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
    duk_unity_get3f(ctx, 1, &rhsx, &rhsy, &rhsz);
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, lhsx + rhsx);
    duk_push_number(ctx, lhsy + rhsy);
    duk_push_number(ctx, lhsz + rhsz);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_Sub(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    float rhsx, rhsy, rhsz;
    duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
    duk_unity_get3f(ctx, 1, &rhsx, &rhsy, &rhsz);
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, lhsx - rhsx);
    duk_push_number(ctx, lhsy - rhsy);
    duk_push_number(ctx, lhsz - rhsz);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_Neg(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, -lhsx);
    duk_push_number(ctx, -lhsy);
    duk_push_number(ctx, -lhsz);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_Mul(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    float f;
    if (duk_is_number(ctx, 0)) {
        f = (float)duk_get_number_default(ctx, 0, 0.0);
        duk_unity_get3f(ctx, 1, &lhsx, &lhsy, &lhsz);
    } else {
        duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
        f = (float)duk_get_number_default(ctx, 1, 0.0);
    }
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, lhsx * f);
    duk_push_number(ctx, lhsy * f);
    duk_push_number(ctx, lhsz * f);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_Inverse(duk_context *ctx) {
    float rhsx, rhsy, rhsz;
    duk_push_this(ctx);
    duk_unity_get3f(ctx, -1, &rhsx, &rhsy, &rhsz);
    duk_pop(ctx);
    float x = 1.0f / rhsx;
    float y = 1.0f / rhsy;
    float z = 1.0f / rhsz;
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, x);
    duk_push_number(ctx, y);
    duk_push_number(ctx, z);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_Equals(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    float rhsx, rhsy, rhsz;
    duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
    duk_unity_get3f(ctx, 1, &rhsx, &rhsy, &rhsz);
    float x = lhsx - rhsx;
    float y = lhsy - rhsy;
    float z = lhsz - rhsz;
    duk_push_boolean(ctx, (x * x + y * y + z * z) < UNITY_VECTOR3_kEpsilon * UNITY_VECTOR3_kEpsilon);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_Equals(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
    float rhsx, rhsy, rhsz;
    duk_push_this(ctx);
    duk_unity_get3f(ctx, -1, &rhsx, &rhsy, &rhsz);
    duk_pop(ctx);
    float x = lhsx - rhsx;
    float y = lhsy - rhsy;
    float z = lhsz - rhsz;
    duk_push_boolean(ctx, (x * x + y * y + z * z) < UNITY_VECTOR3_kEpsilon * UNITY_VECTOR3_kEpsilon);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_Div(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    float f;
    if (duk_is_number(ctx, 0)) {
        f = 1.0f / (float)duk_get_number_default(ctx, 0, 0.0);
        duk_unity_get3f(ctx, 1, &lhsx, &lhsy, &lhsz);
    } else {
        duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
        f = 1.0f / (float)duk_get_number_default(ctx, 1, 0.0);
    }
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, lhsx * f);
    duk_push_number(ctx, lhsy * f);
    duk_push_number(ctx, lhsz * f);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_set(duk_context *ctx) {
    float ax, ay, az;
    ax = (float)duk_get_number_default(ctx, 0, 0.0);
    ay = (float)duk_get_number_default(ctx, 1, 0.0);
    az = (float)duk_get_number_default(ctx, 2, 0.0);
    duk_push_this(ctx);
    duk_unity_put3f(ctx, -1, ax, ay, az);
    duk_pop(ctx);
    return 0;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_scale(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    float rhsx, rhsy, rhsz;
    duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
    duk_push_this(ctx);
    duk_unity_get3f(ctx, -1, &rhsx, &rhsy, &rhsz);
    duk_unity_put3f(ctx, -1, lhsx * rhsx, lhsy * rhsy, lhsz * rhsz);
    duk_pop(ctx);
    return 0;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_scale(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    float rhsx, rhsy, rhsz;
    duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
    duk_unity_get3f(ctx, 1, &rhsx, &rhsy, &rhsz);
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, lhsx * rhsx);
    duk_push_number(ctx, lhsy * rhsy);
    duk_push_number(ctx, lhsz * rhsz);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_cross(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    float rhsx, rhsy, rhsz;
    duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
    duk_unity_get3f(ctx, 1, &rhsx, &rhsy, &rhsz);
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, lhsy * rhsz - lhsz * rhsy);
    duk_push_number(ctx, lhsz * rhsx - lhsx * rhsz);
    duk_push_number(ctx, lhsx * rhsy - lhsy * rhsx);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_OrthoNormalize(duk_context *ctx) {
    float u[3] = {0};
    float v[3] = {0};
    duk_unity_get3f(ctx, 0, &u[0], &u[1], &u[2]);
    duk_unity_get3f(ctx, 1, &v[0], &v[1], &v[2]);
    float mag = vec3_magnitude(u);
    if (mag > UNITY_VECTOR3_kEpsilon) {
        u[0] /= mag;
        u[1] /= mag;
        u[2] /= mag;
    } else {
        u[0] = 1;
        u[1] = u[2] = 0;
    }
    duk_unity_put3f(ctx, 0, u[0], u[1], u[2]);
    float dot0 = vec3_dot(u, v);
    v[0] -= u[0] * dot0;
    v[1] -= u[1] * dot0;
    v[2] -= u[2] * dot0;
    mag = vec3_magnitude(v);
    if (mag > UNITY_VECTOR3_kEpsilon) {
        v[0] /= mag;
        v[1] /= mag;
        v[2] /= mag;
    } else {
        v[0] = v[1] = v[2] = 0;
    }
    duk_unity_put3f(ctx, 1, v[0], v[1], v[2]);

    if (duk_get_top(ctx) > 2) {
        float w[3] = {0};
        duk_unity_get3f(ctx, 2, &w[0], &w[1], &w[2]);
        float dot1 = vec3_dot(v, w);
        dot0 = vec3_dot(u, w);
        w[0] -= u[0] * dot0 + v[0] * dot1;
        w[1] -= u[1] * dot0 + v[1] * dot1;
        w[2] -= u[2] * dot0 + v[2] * dot1;
        mag = vec3_magnitude(w);
        if (mag > UNITY_VECTOR3_kEpsilon) {
            w[0] /= mag;
            w[1] /= mag;
            w[2] /= mag;
        } else {
            vec3_cross(u, v, w);
        }
        duk_unity_put3f(ctx, 2, w[0], w[1], w[2]);
    }
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_Slerp(duk_context *ctx) {
    float lhs[3] = {0};
    float rhs[3] = {0};
    float t;
    duk_unity_get3f(ctx, 0, &lhs[0], &lhs[1], &lhs[2]);
    duk_unity_get3f(ctx, 1, &rhs[0], &rhs[1], &rhs[2]);
    t = (float)duk_get_number_default(ctx, 2, 0);
    // t = t > 1.0f ? 1.0f : (t < 0.0f ? 0.0f : t);
    float lhsMag = sqrtf(lhs[0] * lhs[0] + lhs[1] * lhs[1] + lhs[2] * lhs[2]);
    float rhsMag = sqrtf(rhs[0] * rhs[0] + rhs[1] * rhs[1] + rhs[2] * rhs[2]);
    if (lhsMag < UNITY_VECTOR3_kEpsilon || rhsMag < UNITY_VECTOR3_kEpsilon) {
        duk_builtins_reg_get(ctx, "Vector3");
        duk_push_number(ctx, lhs[0] + (rhs[0] - lhs[0]) * t);
        duk_push_number(ctx, lhs[1] + (rhs[1] - lhs[1]) * t);
        duk_push_number(ctx, lhs[2] + (rhs[2] - lhs[2]) * t);
        duk_new(ctx, 3);
        return 1;
    }
    float lerpedMag = lhsMag + (rhsMag - lhsMag) * t;
    float dot = (lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2]) / (lhsMag * rhsMag);
    if (dot > 1.0f - UNITY_VECTOR3_kEpsilon) {
        duk_builtins_reg_get(ctx, "Vector3");
        duk_push_number(ctx, lhs[0] + (rhs[0] - lhs[0]) * t);
        duk_push_number(ctx, lhs[1] + (rhs[1] - lhs[1]) * t);
        duk_push_number(ctx, lhs[2] + (rhs[2] - lhs[2]) * t);
        duk_new(ctx, 3);
        return 1;
    } else if (dot < -1.0f + UNITY_VECTOR3_kEpsilon) {
        float lhsNorm[3] = {0};
        lhsNorm[0] = lhs[0] / lhsMag;
        lhsNorm[1] = lhs[1] / lhsMag;
        lhsNorm[2] = lhs[2] / lhsMag;
        float axis[3];
        if (fabsf(lhsNorm[2]) > UNITY_VECTOR3_k1OverSqrt2) {
            float k = 1.0f / sqrtf(lhsNorm[1] * lhsNorm[1] + lhsNorm[2] * lhsNorm[2]);
            axis[0] = 0;
            axis[1] = -lhsNorm[2] * k;
            axis[2] = lhsNorm[1] * k;
        } else {
            float k = 1.0f / sqrtf(lhsNorm[0] * lhsNorm[0] + lhsNorm[1] * lhsNorm[1]);
            axis[0] = -lhsNorm[1] * k;
            axis[1] = lhsNorm[0] * k;
            axis[2] = 0;
        }
        float m[9] = {0};
        m3x3_set_axis_angle(m, axis, UNITY_MATH_PI * t);
        float slerped[3] = {0};
        m3x3_multiply_vec3(m, lhsNorm, slerped);
        vec3_multiply(slerped, lerpedMag);
		duk_builtins_reg_get(ctx, "Vector3");
        duk_push_number(ctx, slerped[0]);
        duk_push_number(ctx, slerped[1]);
        duk_push_number(ctx, slerped[2]);
        duk_new(ctx, 3);
        return 1;
    } else {
        float axis[3] = {0};
        vec3_cross(lhs, rhs, axis);
        vec3_normalize(axis);
        float lhsNorm[3] = {0};
        lhsNorm[0] = lhs[0] / lhsMag;
        lhsNorm[1] = lhs[1] / lhsMag;
        lhsNorm[2] = lhs[2] / lhsMag;
		float angle = acosf(dot) * t;
		
        float m[9] = {0};
        m3x3_set_axis_angle(m, axis, angle);
        float slerped[3] = {0};
        m3x3_multiply_vec3(m, lhsNorm, slerped);
		vec3_multiply(slerped, lerpedMag);
		duk_builtins_reg_get(ctx, "Vector3");
        duk_push_number(ctx, slerped[0]);
        duk_push_number(ctx, slerped[1]);
        duk_push_number(ctx, slerped[2]);
        duk_new(ctx, 3);
        return 1;
    }
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_reflect(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    float rhsx, rhsy, rhsz;
    duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
    duk_unity_get3f(ctx, 1, &rhsx, &rhsy, &rhsz);
    float dot2 = 2.0f * (lhsx * rhsx + lhsy * rhsy + lhsz * rhsz);
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, dot2 * rhsx + lhsx);
    duk_push_number(ctx, dot2 * rhsy + lhsy);
    duk_push_number(ctx, dot2 * rhsz + lhsz);
    duk_new(ctx, 3);    
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_normalize(duk_context *ctx) {
    float x, y, z;
    float mag;
    duk_push_this(ctx);
    duk_unity_get3f(ctx, -1, &x, &y, &z);
    mag = sqrtf(x * x + y * y + z * z);
    if (mag > UNITY_VECTOR3_kEpsilon) {
        float rmag = 1.0f / mag;
        duk_unity_put3f(ctx, -1, x * rmag, y * rmag, z * rmag);
    } else {
        duk_unity_put3f(ctx, -1, 0, 0, 0);
    }
    duk_pop(ctx);
    return 0;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_normalized(duk_context *ctx) {
    float x, y, z;
    float mag;
    duk_push_this(ctx);
    duk_unity_get3f(ctx, -1, &x, &y, &z);
    duk_pop(ctx);
    mag = sqrtf(x * x + y * y + z * z);
    if (mag > UNITY_VECTOR3_kEpsilon) {
        float rmag = 1.0f / mag;
        duk_builtins_reg_get(ctx, "Vector3");
        duk_push_number(ctx, x * rmag);
        duk_push_number(ctx, y * rmag);
        duk_push_number(ctx, z * rmag);
        duk_new(ctx, 3);
        return 1;
    }
    duk_builtins_reg_get(ctx, "Vector3");
    duk_get_prop_string(ctx, -1, "zero");
    return 1;
}

/*
public static float Dot(Vector3 lhs, Vector3 rhs)
*/
DUK_LOCAL duk_ret_t duk_unity_vector3_static_dot(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    float rhsx, rhsy, rhsz;
    duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
    duk_unity_get3f(ctx, 1, &rhsx, &rhsy, &rhsz);
    duk_push_number(ctx, lhsx * rhsx + lhsy * rhsy + lhsz * rhsz);
    return 1;
}

/*
public static Vector3 Project(Vector3 vector, Vector3 onNormal)
*/
DUK_LOCAL duk_ret_t duk_unity_vector3_static_project(duk_context *ctx) {
    float nx, ny, nz;
    duk_unity_get3f(ctx, 1, &nx, &ny, &nz);
    float sqrMag = nx * nx + ny * ny + nz * nz;
    if (sqrMag < UNITY_SINGLE_Epsilon) {
        duk_builtins_reg_get(ctx, "Vector3");
        duk_get_prop_string(ctx, -1, "zero");
        return 1;
    }
    float vx, vy, vz;
    duk_unity_get3f(ctx, 0, &vx, &vy, &vz);
    float dotsq = (vx * nx + vy * ny + vz * nz) / sqrMag;
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, nx * dotsq);
    duk_push_number(ctx, ny * dotsq);
    duk_push_number(ctx, nz * dotsq);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_projectOnPlane(duk_context *ctx) {
    float nx, ny, nz;
    float vx, vy, vz;
    duk_unity_get3f(ctx, 0, &vx, &vy, &vz);
    duk_unity_get3f(ctx, 1, &nx, &ny, &nz);
    float sqrMag = nx * nx + ny * ny + nz * nz;
    if (sqrMag < UNITY_SINGLE_Epsilon) {
        duk_dup(ctx, 0);
        return 1;
    }
    float dotsq = (vx * nx + vy * ny + vz * nz) / sqrMag;
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, vx - nx * dotsq);
    duk_push_number(ctx, vy - ny * dotsq);
    duk_push_number(ctx, vz - nz * dotsq);
    duk_new(ctx, 3);
    return 1;
}

/*
public static float Angle(Vector3 from, Vector3 to)
*/
DUK_LOCAL duk_ret_t duk_unity_vector3_static_angle(duk_context *ctx) {
    float from_x, from_y, from_z;
    float to_x, to_y, to_z;
    duk_unity_get3f(ctx, 0, &from_x, &from_y, &from_z);
    duk_unity_get3f(ctx, 1, &to_x, &to_y, &to_z);
    float denominator = sqrtf((from_x * from_x + from_y * from_y + from_z * from_z) * (to_x * to_x + to_y * to_y + to_z * to_z));
    if (denominator < UNITY_VECTOR3_kEpsilonNormalSqrt) {
        duk_push_number(ctx, 0);
        return 1;
    }
    float dot = (from_x * to_x + from_y * to_y + from_z * to_z) / denominator;
    if (dot > 1) {
        dot = 1;
    } else if (dot < -1) {
        dot = -1;
    }
    float ret = acosf(dot) / (3.1415926535897931f * 2.0f / 360.0f);
    duk_push_number(ctx, ret);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_signedAngle(duk_context *ctx) {
    float from_x, from_y, from_z;
    float to_x, to_y, to_z;
    duk_unity_get3f(ctx, 0, &from_x, &from_y, &from_z);
    duk_unity_get3f(ctx, 1, &to_x, &to_y, &to_z);
    float denominator = sqrtf((from_x * from_x + from_y * from_y + from_z * from_z) * (to_x * to_x + to_y * to_y + to_z * to_z));
    if (denominator < UNITY_VECTOR3_kEpsilonNormalSqrt) {
        duk_push_number(ctx, 0);
        return 1;
    }
    float axis_x, axis_y, axis_z;
    duk_unity_get3f(ctx, 2, &axis_x, &axis_y, &axis_z);
    float dot = (from_x * to_x + from_y * to_y + from_z * to_z) / denominator;
    if (dot > 1) {
        dot = 1;
    } else if (dot < -1) {
        dot = -1;
    }
    float ret = acosf(dot) / (3.1415926535897931f * 2.0f / 360.0f);
    float cross_x = from_y * to_z - from_z * to_y;
    float cross_y = from_z * to_x - from_x * to_z;
    float cross_z = from_x * to_y - from_y * to_x;
    float dot_axis = axis_x * cross_x + axis_y * cross_y + axis_z * cross_z;
    duk_push_number(ctx, dot_axis > 0.0f ? ret : -ret);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_distance(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    float rhsx, rhsy, rhsz;
    duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
    duk_unity_get3f(ctx, 1, &rhsx, &rhsy, &rhsz);
    float x = lhsx - rhsx;
    float y = lhsy - rhsy;
    float z = lhsz - rhsz;
    duk_push_number(ctx, sqrtf(x * x + y * y + z * z));
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_static_clampMagnitude(duk_context *ctx) {
    float x, y, z;
    float maxLength;
    duk_unity_get3f(ctx, 0, &x, &y, &z);
    maxLength = (float)duk_get_number_default(ctx, 1, 0);
    float sqrMag = x * x + y * y + z * z;
    if (sqrMag > maxLength * maxLength) {
        float rx = maxLength / sqrMag;
        duk_builtins_reg_get(ctx, "Vector3");
        duk_push_number(ctx, x * rx);
        duk_push_number(ctx, y * rx);
        duk_push_number(ctx, z * rx);
        duk_new(ctx, 3);
        return 1;
    }
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, x);
    duk_push_number(ctx, y);
    duk_push_number(ctx, z);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_magnitude(duk_context *ctx) {
    float x, y, z;
    duk_push_this(ctx);
    duk_unity_get3f(ctx, -1, &x, &y, &z);
    duk_push_number(ctx, sqrtf(x * x + y * y + z * z));
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_sqrMagnitude(duk_context *ctx) {
    float x, y, z;
    duk_push_this(ctx);
    duk_unity_get3f(ctx, -1, &x, &y, &z);
    duk_push_number(ctx, x * x + y * y + z * z);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_min(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    float rhsx, rhsy, rhsz;
    duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
    duk_unity_get3f(ctx, 1, &rhsx, &rhsy, &rhsz);
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, lhsx > rhsx ? rhsx : lhsx);
    duk_push_number(ctx, lhsy > rhsy ? rhsy : lhsy);
    duk_push_number(ctx, lhsz > rhsz ? rhsz : lhsz);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_max(duk_context *ctx) {
    float lhsx, lhsy, lhsz;
    float rhsx, rhsy, rhsz;
    duk_unity_get3f(ctx, 0, &lhsx, &lhsy, &lhsz);
    duk_unity_get3f(ctx, 1, &rhsx, &rhsy, &rhsz);
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, lhsx < rhsx ? rhsx : lhsx);
    duk_push_number(ctx, lhsy < rhsy ? rhsy : lhsy);
    duk_push_number(ctx, lhsz < rhsz ? rhsz : lhsz);
    duk_new(ctx, 3);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_getx(duk_context *ctx) {
    duk_push_this(ctx);
    duk_get_prop_index(ctx, -1, 0);
    duk_remove(ctx, -2);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_setx(duk_context *ctx) {
    duk_push_this(ctx);
    duk_dup(ctx, 0);
    duk_put_prop_index(ctx, -2, 0);
    duk_pop(ctx);
    return 0;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_gety(duk_context *ctx) {
    duk_push_this(ctx);
    duk_get_prop_index(ctx, -1, 1);
    duk_remove(ctx, -2);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_sety(duk_context *ctx) {
    duk_push_this(ctx);
    duk_dup(ctx, 0);
    duk_put_prop_index(ctx, -2, 1);
    duk_pop(ctx);
    return 0;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_getz(duk_context *ctx) {
    duk_push_this(ctx);
    duk_get_prop_index(ctx, -1, 2);
    duk_remove(ctx, -2);
    return 1;
}

DUK_LOCAL duk_ret_t duk_unity_vector3_setz(duk_context *ctx) {
    duk_push_this(ctx);
    duk_dup(ctx, 0);
    duk_put_prop_index(ctx, -2, 2);
    duk_pop(ctx);
    return 0;
}

DUK_LOCAL void duk_unity_vector3_add_const(duk_context *ctx, duk_idx_t idx, const char *key, float x, float y, float z) {
    idx = duk_normalize_index(ctx, idx);
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, x);
    duk_push_number(ctx, y);
    duk_push_number(ctx, z);
    duk_new(ctx, 3);
    duk_put_prop_string(ctx, idx, key);
}

DUK_EXTERNAL void duk_unity_push_vector3(duk_context *ctx, float v1, float v2, float v3) {
    duk_builtins_reg_get(ctx, "Vector3");
    duk_push_number(ctx, v1);
    duk_push_number(ctx, v2);
    duk_push_number(ctx, 3);
    duk_new(ctx, 3);
}

DUK_INTERNAL void duk_unity_vector3_open(duk_context *ctx) {
    duk_push_global_object(ctx);
    duk_unity_get_prop_object(ctx, -1, "DuktapeJS");
    duk_unity_begin_class(ctx, "Vector3", duk_unity_vector3_constructor, NULL);

    duk_unity_add_member(ctx, "Add", duk_unity_vector3_static_Add, -2);
    duk_unity_add_member(ctx, "Sub", duk_unity_vector3_static_Sub, -2);
    duk_unity_add_member(ctx, "Neg", duk_unity_vector3_static_Neg, -2);
    duk_unity_add_member(ctx, "Mul", duk_unity_vector3_static_Mul, -2);
    duk_unity_add_member(ctx, "Div", duk_unity_vector3_static_Div, -2);
    duk_unity_add_member(ctx, "Inverse", duk_unity_vector3_Inverse, -1);
    duk_unity_add_member(ctx, "Equals", duk_unity_vector3_static_Equals, -2);
    duk_unity_add_member(ctx, "Equals", duk_unity_vector3_Equals, -1);

    duk_unity_add_member(ctx, "Lerp", duk_unity_vector3_static_lerp, -1);
    duk_unity_add_member(ctx, "LerpUnclamped", duk_unity_vector3_static_lerpUnclamped, -2);
    duk_unity_add_member(ctx, "MoveTowards", duk_unity_vector3_static_moveTowards, -2);
    duk_unity_add_member(ctx, "SmoothDamp", duk_unity_vector3_static_smoothDamp, -2);
    duk_unity_add_member(ctx, "Scale", duk_unity_vector3_scale, -1);
    duk_unity_add_member(ctx, "Scale", duk_unity_vector3_static_scale, -2);
    duk_unity_add_member(ctx, "Cross", duk_unity_vector3_static_cross, -2);
    duk_unity_add_member(ctx, "Slerp", duk_unity_vector3_static_Slerp, -2);
    duk_unity_add_member(ctx, "OrthoNormalize", duk_unity_vector3_static_OrthoNormalize, -2);
    duk_unity_add_member(ctx, "Reflect", duk_unity_vector3_static_reflect, -2);
    duk_unity_add_member(ctx, "Normalize", duk_unity_vector3_normalize, -1);
    duk_unity_add_property(ctx, "normalized", duk_unity_vector3_normalized, NULL, -1);
    duk_unity_add_member(ctx, "Dot", duk_unity_vector3_static_dot, -2);
    duk_unity_add_member(ctx, "Project", duk_unity_vector3_static_project, -2);
    duk_unity_add_member(ctx, "ProjectOnPlane", duk_unity_vector3_static_projectOnPlane, -2);
    duk_unity_add_member(ctx, "Angle", duk_unity_vector3_static_angle, -2);
    duk_unity_add_member(ctx, "SignedAngle", duk_unity_vector3_static_signedAngle, -2);
    duk_unity_add_member(ctx, "Distance", duk_unity_vector3_static_distance, -2);
    duk_unity_add_member(ctx, "ClampMagnitude", duk_unity_vector3_static_clampMagnitude, -2);
    duk_unity_add_property(ctx, "magnitude", duk_unity_vector3_magnitude, NULL, -1);
    duk_unity_add_property(ctx, "sqrMagnitude", duk_unity_vector3_sqrMagnitude, NULL, -1);
    duk_unity_add_member(ctx, "Min", duk_unity_vector3_min, -1);
    duk_unity_add_member(ctx, "Max", duk_unity_vector3_max, -1);
    duk_unity_add_property(ctx, "x", duk_unity_vector3_getx, duk_unity_vector3_setx, -1);
    duk_unity_add_property(ctx, "y", duk_unity_vector3_gety, duk_unity_vector3_sety, -1);
    duk_unity_add_property(ctx, "z", duk_unity_vector3_getz, duk_unity_vector3_setz, -1);
    duk_unity_vector3_add_const(ctx, -2, "zero", 0.0, 0.0, 0.0);
    duk_unity_vector3_add_const(ctx, -2, "one", 1.0, 1.0, 1.0);
    duk_unity_vector3_add_const(ctx, -2, "forward", 0.0, 0.0, 1.0);
    duk_unity_vector3_add_const(ctx, -2, "back", 0.0, 0.0, -1.0);
    duk_unity_vector3_add_const(ctx, -2, "up", 0.0, 1.0, 0.0);
    duk_unity_vector3_add_const(ctx, -2, "down", 0.0, -1.0, 0.0);
    duk_unity_vector3_add_const(ctx, -2, "left", -1.0, 0.0, 0.0);
    duk_unity_vector3_add_const(ctx, -2, "right", 1.0, 0.0, 0.0);
    duk_unity_end_class(ctx);
    duk_pop_2(ctx); // pop DuktapeJS and global
}