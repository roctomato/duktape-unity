using System;
using System.Collections.Generic;
using AOT;

namespace Duktape
{
    using UnityEngine;

    // 处理类型匹配
    public partial class DuktapeBinding
    {
        protected static HashSet<Type> _assignableFromArray = new HashSet<Type>();

        static DuktapeBinding()
        {
            _assignableFromArray.Add(typeof(LayerMask));
            _assignableFromArray.Add(typeof(Color));
            _assignableFromArray.Add(typeof(Color32));
            _assignableFromArray.Add(typeof(Vector2));
            _assignableFromArray.Add(typeof(Vector2Int));
            _assignableFromArray.Add(typeof(Vector3));
            _assignableFromArray.Add(typeof(Vector3Int));
            _assignableFromArray.Add(typeof(Vector4));
            _assignableFromArray.Add(typeof(Quaternion));
            // _assignableFromArray.Add(typeof(Matrix4x4));
        }

        protected static bool duk_match_type(IntPtr ctx, int idx, Type type)
        {
            if (type == typeof(object))
            {
                return true;
            }
            var jstype = DuktapeDLL.duk_get_type(ctx, idx);
            switch (jstype)
            {
                // case duk_type_t.DUK_TYPE_NONE:
                case duk_type_t.DUK_TYPE_OBJECT: // objects, arrays, functions, threads
                    if (DuktapeDLL.duk_is_array(ctx, idx))
                    {
                        if (!type.IsArray && !_assignableFromArray.Contains(type))
                        {
                            return false;
                        }
                    }
                    else if (DuktapeDLL.duk_is_function(ctx, idx))
                    {
                        //TODO: 完善处理 delegate 
                        return type == typeof(DuktapeFunction) || type.BaseType == typeof(MulticastDelegate);
                    }
                    else if (DuktapeDLL.duk_is_thread(ctx, idx))
                    {
                        return false;
                    }
                    int refid;
                    //TODO: 根据记录在jsobject 上的 分类标记 做进一步分支 (类型, 实例, 或特定优化类型)
                    if (duk_get_native_refid(ctx, idx, out refid))
                    {
                        var cache = DuktapeVM.GetObjectCache(ctx);
                        return cache.MatchType(refid, type);
                    }
                    return true;
                case duk_type_t.DUK_TYPE_NUMBER:
                    return type.IsPrimitive || type.IsEnum;
                case duk_type_t.DUK_TYPE_STRING:
                    return type == typeof(string);
                case duk_type_t.DUK_TYPE_UNDEFINED:
                case duk_type_t.DUK_TYPE_NULL:
                    return !type.IsValueType && !type.IsPrimitive;
                case duk_type_t.DUK_TYPE_BOOLEAN:
                    return type == typeof(bool);
                case duk_type_t.DUK_TYPE_BUFFER:
                    return type == typeof(byte[]) || type == typeof(sbyte[]) /* || type == typeof(DuktapeBuffer) */;
                case duk_type_t.DUK_TYPE_POINTER:
                // return type == typeof(DuktapePointer);
                case duk_type_t.DUK_TYPE_LIGHTFUNC:
                default:
                    return false;
            }
        }

        protected static bool duk_match_types(IntPtr ctx, int nargs, Type t0)
        {
            return true;
        }

        protected static bool duk_match_types(IntPtr ctx, int nargs, Type t0, Type t1)
        {
            return true;
        }

        protected static bool duk_match_types(IntPtr ctx, int nargs, Type t0, Type t1, Type t2)
        {
            return true;
        }

        protected static bool duk_match_types(IntPtr ctx, int nargs, Type t0, Type t1, Type t2, Type t3)
        {
            return true;
        }

        protected static bool duk_match_types(IntPtr ctx, int nargs, Type t0, Type t1, Type t2, Type t3, Type t4)
        {
            return true;
        }

        protected static bool duk_match_types(IntPtr ctx, int nargs, params Type[] types)
        {
            return true;
        }
    }
}