/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"

#if ENABLE(SVG)

#include "JSSVGPaint.h"

#include "ExceptionCode.h"
#include "JSDOMBinding.h"
#include "KURL.h"
#include "SVGPaint.h"
#include <runtime/Error.h>
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSSVGPaint);

/* Hash table */
#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

static const HashTableValue JSSVGPaintTableValues[] =
{
    { "paintType", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintPaintType), (intptr_t)0 THUNK_GENERATOR(0) },
    { "uri", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintUri), (intptr_t)0 THUNK_GENERATOR(0) },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintConstructor), (intptr_t)0 THUNK_GENERATOR(0) },
    { 0, 0, 0, 0 THUNK_GENERATOR(0) }
};

#undef THUNK_GENERATOR
static JSC_CONST_HASHTABLE HashTable JSSVGPaintTable = { 9, 7, JSSVGPaintTableValues, 0 };
/* Hash table for constructor */
#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

static const HashTableValue JSSVGPaintConstructorTableValues[] =
{
    { "SVG_PAINTTYPE_UNKNOWN", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_UNKNOWN), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_RGBCOLOR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_RGBCOLOR), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_RGBCOLOR_ICCCOLOR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_RGBCOLOR_ICCCOLOR), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_NONE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_NONE), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_CURRENTCOLOR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_CURRENTCOLOR), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_URI_NONE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_URI_NONE), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_URI_CURRENTCOLOR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_URI_CURRENTCOLOR), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_URI_RGBCOLOR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_URI_RGBCOLOR), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_URI", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_URI), (intptr_t)0 THUNK_GENERATOR(0) },
    { 0, 0, 0, 0 THUNK_GENERATOR(0) }
};

#undef THUNK_GENERATOR
static JSC_CONST_HASHTABLE HashTable JSSVGPaintConstructorTable = { 35, 31, JSSVGPaintConstructorTableValues, 0 };

COMPILE_ASSERT(0 == SVGPaint::SVG_PAINTTYPE_UNKNOWN, SVGPaintEnumSVG_PAINTTYPE_UNKNOWNIsWrongUseDontCheckEnums);
COMPILE_ASSERT(1 == SVGPaint::SVG_PAINTTYPE_RGBCOLOR, SVGPaintEnumSVG_PAINTTYPE_RGBCOLORIsWrongUseDontCheckEnums);
COMPILE_ASSERT(2 == SVGPaint::SVG_PAINTTYPE_RGBCOLOR_ICCCOLOR, SVGPaintEnumSVG_PAINTTYPE_RGBCOLOR_ICCCOLORIsWrongUseDontCheckEnums);
COMPILE_ASSERT(101 == SVGPaint::SVG_PAINTTYPE_NONE, SVGPaintEnumSVG_PAINTTYPE_NONEIsWrongUseDontCheckEnums);
COMPILE_ASSERT(102 == SVGPaint::SVG_PAINTTYPE_CURRENTCOLOR, SVGPaintEnumSVG_PAINTTYPE_CURRENTCOLORIsWrongUseDontCheckEnums);
COMPILE_ASSERT(103 == SVGPaint::SVG_PAINTTYPE_URI_NONE, SVGPaintEnumSVG_PAINTTYPE_URI_NONEIsWrongUseDontCheckEnums);
COMPILE_ASSERT(104 == SVGPaint::SVG_PAINTTYPE_URI_CURRENTCOLOR, SVGPaintEnumSVG_PAINTTYPE_URI_CURRENTCOLORIsWrongUseDontCheckEnums);
COMPILE_ASSERT(105 == SVGPaint::SVG_PAINTTYPE_URI_RGBCOLOR, SVGPaintEnumSVG_PAINTTYPE_URI_RGBCOLORIsWrongUseDontCheckEnums);
COMPILE_ASSERT(106 == SVGPaint::SVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR, SVGPaintEnumSVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLORIsWrongUseDontCheckEnums);
COMPILE_ASSERT(107 == SVGPaint::SVG_PAINTTYPE_URI, SVGPaintEnumSVG_PAINTTYPE_URIIsWrongUseDontCheckEnums);

class JSSVGPaintConstructor : public DOMConstructorObject {
private:
    JSSVGPaintConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSSVGPaintConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSSVGPaintConstructor* ptr = new (JSC::allocateCell<JSSVGPaintConstructor>(*exec->heap())) JSSVGPaintConstructor(structure, globalObject);
        ptr->finishCreation(exec, globalObject);
        return ptr;
    }

    virtual bool getOwnPropertySlot(JSC::ExecState*, const JSC::Identifier&, JSC::PropertySlot&);
    virtual bool getOwnPropertyDescriptor(JSC::ExecState*, const JSC::Identifier&, JSC::PropertyDescriptor&);
    static const JSC::ClassInfo s_info;
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::ImplementsHasInstance | DOMConstructorObject::StructureFlags;
};

const ClassInfo JSSVGPaintConstructor::s_info = { "SVGPaintConstructor", &DOMConstructorObject::s_info, &JSSVGPaintConstructorTable, 0 };

JSSVGPaintConstructor::JSSVGPaintConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSSVGPaintConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSSVGPaintPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSSVGPaintConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGPaintConstructor, JSDOMWrapper>(exec, &JSSVGPaintConstructorTable, this, propertyName, slot);
}

bool JSSVGPaintConstructor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGPaintConstructor, JSDOMWrapper>(exec, &JSSVGPaintConstructorTable, this, propertyName, descriptor);
}

/* Hash table for prototype */
#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

static const HashTableValue JSSVGPaintPrototypeTableValues[] =
{
    { "SVG_PAINTTYPE_UNKNOWN", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_UNKNOWN), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_RGBCOLOR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_RGBCOLOR), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_RGBCOLOR_ICCCOLOR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_RGBCOLOR_ICCCOLOR), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_NONE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_NONE), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_CURRENTCOLOR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_CURRENTCOLOR), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_URI_NONE", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_URI_NONE), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_URI_CURRENTCOLOR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_URI_CURRENTCOLOR), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_URI_RGBCOLOR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_URI_RGBCOLOR), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR), (intptr_t)0 THUNK_GENERATOR(0) },
    { "SVG_PAINTTYPE_URI", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPaintSVG_PAINTTYPE_URI), (intptr_t)0 THUNK_GENERATOR(0) },
    { "setUri", DontDelete | Function, (intptr_t)static_cast<NativeFunction>(jsSVGPaintPrototypeFunctionSetUri), (intptr_t)1 THUNK_GENERATOR(0) },
    { "setPaint", DontDelete | Function, (intptr_t)static_cast<NativeFunction>(jsSVGPaintPrototypeFunctionSetPaint), (intptr_t)4 THUNK_GENERATOR(0) },
    { 0, 0, 0, 0 THUNK_GENERATOR(0) }
};

#undef THUNK_GENERATOR
static JSC_CONST_HASHTABLE HashTable JSSVGPaintPrototypeTable = { 35, 31, JSSVGPaintPrototypeTableValues, 0 };
const ClassInfo JSSVGPaintPrototype::s_info = { "SVGPaintPrototype", &JSC::JSNonFinalObject::s_info, &JSSVGPaintPrototypeTable, 0 };

JSObject* JSSVGPaintPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGPaint>(exec, globalObject);
}

bool JSSVGPaintPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticPropertySlot<JSSVGPaintPrototype, JSObject>(exec, &JSSVGPaintPrototypeTable, this, propertyName, slot);
}

bool JSSVGPaintPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticPropertyDescriptor<JSSVGPaintPrototype, JSObject>(exec, &JSSVGPaintPrototypeTable, this, propertyName, descriptor);
}

const ClassInfo JSSVGPaint::s_info = { "SVGPaint", &JSSVGColor::s_info, &JSSVGPaintTable, 0 };

JSSVGPaint::JSSVGPaint(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGPaint> impl)
    : JSSVGColor(structure, globalObject, impl)
{
}

void JSSVGPaint::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSSVGPaint::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSSVGPaintPrototype::create(exec->globalData(), globalObject, JSSVGPaintPrototype::createStructure(exec->globalData(), globalObject, JSSVGColorPrototype::self(exec, globalObject)));
}

bool JSSVGPaint::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    ASSERT_GC_OBJECT_INHERITS(this, &s_info);
    return getStaticValueSlot<JSSVGPaint, Base>(exec, &JSSVGPaintTable, this, propertyName, slot);
}

bool JSSVGPaint::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    ASSERT_GC_OBJECT_INHERITS(this, &s_info);
    return getStaticValueDescriptor<JSSVGPaint, Base>(exec, &JSSVGPaintTable, this, propertyName, descriptor);
}

JSValue jsSVGPaintPaintType(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPaint* castedThis = static_cast<JSSVGPaint*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPaint* imp = static_cast<SVGPaint*>(castedThis->impl());
    JSValue result = jsNumber(imp->paintType());
    return result;
}


JSValue jsSVGPaintUri(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPaint* castedThis = static_cast<JSSVGPaint*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPaint* imp = static_cast<SVGPaint*>(castedThis->impl());
    JSValue result = jsString(exec, imp->uri());
    return result;
}


JSValue jsSVGPaintConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPaint* domObject = static_cast<JSSVGPaint*>(asObject(slotBase));
    return JSSVGPaint::getConstructor(exec, domObject->globalObject());
}

JSValue JSSVGPaint::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGPaintConstructor>(exec, static_cast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsSVGPaintPrototypeFunctionSetUri(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGPaint::s_info))
        return throwVMTypeError(exec);
    JSSVGPaint* castedThis = static_cast<JSSVGPaint*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGPaint::s_info);
    SVGPaint* imp = static_cast<SVGPaint*>(castedThis->impl());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createTypeError(exec, "Not enough arguments"));
    const String& uri(ustringToString(exec->argument(0).toString(exec)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    imp->setUri(uri);
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsSVGPaintPrototypeFunctionSetPaint(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSSVGPaint::s_info))
        return throwVMTypeError(exec);
    JSSVGPaint* castedThis = static_cast<JSSVGPaint*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSSVGPaint::s_info);
    SVGPaint* imp = static_cast<SVGPaint*>(castedThis->impl());
    if (exec->argumentCount() < 4)
        return throwVMError(exec, createTypeError(exec, "Not enough arguments"));
    ExceptionCode ec = 0;
    unsigned short paintType(exec->argument(0).toUInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    const String& uri(ustringToString(exec->argument(1).toString(exec)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    const String& rgbColor(ustringToString(exec->argument(2).toString(exec)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    const String& iccColor(ustringToString(exec->argument(3).toString(exec)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    imp->setPaint(paintType, uri, rgbColor, iccColor, ec);
    setDOMException(exec, ec);
    return JSValue::encode(jsUndefined());
}

// Constant getters

JSValue jsSVGPaintSVG_PAINTTYPE_UNKNOWN(ExecState* exec, JSValue, const Identifier&)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(0));
}

JSValue jsSVGPaintSVG_PAINTTYPE_RGBCOLOR(ExecState* exec, JSValue, const Identifier&)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(1));
}

JSValue jsSVGPaintSVG_PAINTTYPE_RGBCOLOR_ICCCOLOR(ExecState* exec, JSValue, const Identifier&)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(2));
}

JSValue jsSVGPaintSVG_PAINTTYPE_NONE(ExecState* exec, JSValue, const Identifier&)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(101));
}

JSValue jsSVGPaintSVG_PAINTTYPE_CURRENTCOLOR(ExecState* exec, JSValue, const Identifier&)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(102));
}

JSValue jsSVGPaintSVG_PAINTTYPE_URI_NONE(ExecState* exec, JSValue, const Identifier&)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(103));
}

JSValue jsSVGPaintSVG_PAINTTYPE_URI_CURRENTCOLOR(ExecState* exec, JSValue, const Identifier&)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(104));
}

JSValue jsSVGPaintSVG_PAINTTYPE_URI_RGBCOLOR(ExecState* exec, JSValue, const Identifier&)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(105));
}

JSValue jsSVGPaintSVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR(ExecState* exec, JSValue, const Identifier&)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(106));
}

JSValue jsSVGPaintSVG_PAINTTYPE_URI(ExecState* exec, JSValue, const Identifier&)
{
    UNUSED_PARAM(exec);
    return jsNumber(static_cast<int>(107));
}


}

#endif // ENABLE(SVG)
