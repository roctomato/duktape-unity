using System;
using System.Collections.Generic;
using AOT;

namespace Duktape
{
    using UnityEngine;

    public class UnityEngine_Object : DuktapeBinding
    {
        [MonoPInvokeCallback(typeof(DuktapeDLL.duk_c_function))]
        static int ctor(IntPtr ctx)
        {
            DuktapeDLL.duk_push_this(ctx);
            var o = new UnityEngine.Object();
            duk_push_any(ctx, o);
            DuktapeDLL.duk_put_prop_string(ctx, -2, DuktapeVM.OBJ_PROP_NATIVE);
            DuktapeDLL.duk_pop(ctx);
            return 0;
        }

        [MonoPInvokeCallback(typeof(DuktapeDLL.duk_c_function))]
        static int Destroy(IntPtr ctx)
        {
            object arg1;
            duk_get_any(ctx, 1, out arg1);
            DuktapeDLL.duk_pop(ctx); // pop this
            var tp = (Object)arg1;
            Object.Destroy(tp);
            return 0;
        }

        [MonoPInvokeCallback(typeof(DuktapeDLL.duk_c_function))]
        static int Foo(IntPtr ctx)
        {
            Debug.Log("Object.Foo");
            return 0;
        }

        public static void reg(IntPtr ctx)
        {
            duk_begin_namespace(ctx, "UnityEngine");
            duk_begin_class(ctx, typeof(UnityEngine.Object), ctor);
            duk_add_method(ctx, "Destroy", Destroy, true);
            duk_add_method(ctx, "Foo", Foo, false);
            duk_end_class(ctx);
            duk_end_namespace(ctx);
        }
    }
}