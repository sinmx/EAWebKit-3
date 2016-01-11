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

#include "JSSVGPathSegCurvetoCubicAbs.h"

#include "SVGPathSegCurvetoCubic.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSSVGPathSegCurvetoCubicAbs);

/* Hash table */
#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

static const HashTableValue JSSVGPathSegCurvetoCubicAbsTableValues[] =
{
    { "x", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoCubicAbsX), (intptr_t)setJSSVGPathSegCurvetoCubicAbsX THUNK_GENERATOR(0) },
    { "y", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoCubicAbsY), (intptr_t)setJSSVGPathSegCurvetoCubicAbsY THUNK_GENERATOR(0) },
    { "x1", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoCubicAbsX1), (intptr_t)setJSSVGPathSegCurvetoCubicAbsX1 THUNK_GENERATOR(0) },
    { "y1", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoCubicAbsY1), (intptr_t)setJSSVGPathSegCurvetoCubicAbsY1 THUNK_GENERATOR(0) },
    { "x2", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoCubicAbsX2), (intptr_t)setJSSVGPathSegCurvetoCubicAbsX2 THUNK_GENERATOR(0) },
    { "y2", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoCubicAbsY2), (intptr_t)setJSSVGPathSegCurvetoCubicAbsY2 THUNK_GENERATOR(0) },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsSVGPathSegCurvetoCubicAbsConstructor), (intptr_t)0 THUNK_GENERATOR(0) },
    { 0, 0, 0, 0 THUNK_GENERATOR(0) }
};

#undef THUNK_GENERATOR
static JSC_CONST_HASHTABLE HashTable JSSVGPathSegCurvetoCubicAbsTable = { 16, 15, JSSVGPathSegCurvetoCubicAbsTableValues, 0 };
/* Hash table for constructor */
#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

static const HashTableValue JSSVGPathSegCurvetoCubicAbsConstructorTableValues[] =
{
    { 0, 0, 0, 0 THUNK_GENERATOR(0) }
};

#undef THUNK_GENERATOR
static JSC_CONST_HASHTABLE HashTable JSSVGPathSegCurvetoCubicAbsConstructorTable = { 1, 0, JSSVGPathSegCurvetoCubicAbsConstructorTableValues, 0 };
class JSSVGPathSegCurvetoCubicAbsConstructor : public DOMConstructorObject {
private:
    JSSVGPathSegCurvetoCubicAbsConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSSVGPathSegCurvetoCubicAbsConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSSVGPathSegCurvetoCubicAbsConstructor* ptr = new (JSC::allocateCell<JSSVGPathSegCurvetoCubicAbsConstructor>(*exec->heap())) JSSVGPathSegCurvetoCubicAbsConstructor(structure, globalObject);
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

const ClassInfo JSSVGPathSegCurvetoCubicAbsConstructor::s_info = { "SVGPathSegCurvetoCubicAbsConstructor", &DOMConstructorObject::s_info, &JSSVGPathSegCurvetoCubicAbsConstructorTable, 0 };

JSSVGPathSegCurvetoCubicAbsConstructor::JSSVGPathSegCurvetoCubicAbsConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSSVGPathSegCurvetoCubicAbsConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSSVGPathSegCurvetoCubicAbsPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSSVGPathSegCurvetoCubicAbsConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGPathSegCurvetoCubicAbsConstructor, JSDOMWrapper>(exec, &JSSVGPathSegCurvetoCubicAbsConstructorTable, this, propertyName, slot);
}

bool JSSVGPathSegCurvetoCubicAbsConstructor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSSVGPathSegCurvetoCubicAbsConstructor, JSDOMWrapper>(exec, &JSSVGPathSegCurvetoCubicAbsConstructorTable, this, propertyName, descriptor);
}

/* Hash table for prototype */
#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

static const HashTableValue JSSVGPathSegCurvetoCubicAbsPrototypeTableValues[] =
{
    { 0, 0, 0, 0 THUNK_GENERATOR(0) }
};

#undef THUNK_GENERATOR
static JSC_CONST_HASHTABLE HashTable JSSVGPathSegCurvetoCubicAbsPrototypeTable = { 1, 0, JSSVGPathSegCurvetoCubicAbsPrototypeTableValues, 0 };
const ClassInfo JSSVGPathSegCurvetoCubicAbsPrototype::s_info = { "SVGPathSegCurvetoCubicAbsPrototype", &JSC::JSNonFinalObject::s_info, &JSSVGPathSegCurvetoCubicAbsPrototypeTable, 0 };

JSObject* JSSVGPathSegCurvetoCubicAbsPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSSVGPathSegCurvetoCubicAbs>(exec, globalObject);
}

const ClassInfo JSSVGPathSegCurvetoCubicAbs::s_info = { "SVGPathSegCurvetoCubicAbs", &JSSVGPathSeg::s_info, &JSSVGPathSegCurvetoCubicAbsTable, 0 };

JSSVGPathSegCurvetoCubicAbs::JSSVGPathSegCurvetoCubicAbs(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SVGPathSegCurvetoCubicAbs> impl)
    : JSSVGPathSeg(structure, globalObject, impl)
{
}

void JSSVGPathSegCurvetoCubicAbs::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSSVGPathSegCurvetoCubicAbs::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSSVGPathSegCurvetoCubicAbsPrototype::create(exec->globalData(), globalObject, JSSVGPathSegCurvetoCubicAbsPrototype::createStructure(exec->globalData(), globalObject, JSSVGPathSegPrototype::self(exec, globalObject)));
}

bool JSSVGPathSegCurvetoCubicAbs::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    ASSERT_GC_OBJECT_INHERITS(this, &s_info);
    return getStaticValueSlot<JSSVGPathSegCurvetoCubicAbs, Base>(exec, &JSSVGPathSegCurvetoCubicAbsTable, this, propertyName, slot);
}

bool JSSVGPathSegCurvetoCubicAbs::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    ASSERT_GC_OBJECT_INHERITS(this, &s_info);
    return getStaticValueDescriptor<JSSVGPathSegCurvetoCubicAbs, Base>(exec, &JSSVGPathSegCurvetoCubicAbsTable, this, propertyName, descriptor);
}

JSValue jsSVGPathSegCurvetoCubicAbsX(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    JSValue result = jsNumber(imp->x());
    return result;
}


JSValue jsSVGPathSegCurvetoCubicAbsY(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    JSValue result = jsNumber(imp->y());
    return result;
}


JSValue jsSVGPathSegCurvetoCubicAbsX1(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    JSValue result = jsNumber(imp->x1());
    return result;
}


JSValue jsSVGPathSegCurvetoCubicAbsY1(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    JSValue result = jsNumber(imp->y1());
    return result;
}


JSValue jsSVGPathSegCurvetoCubicAbsX2(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    JSValue result = jsNumber(imp->x2());
    return result;
}


JSValue jsSVGPathSegCurvetoCubicAbsY2(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    JSValue result = jsNumber(imp->y2());
    return result;
}


JSValue jsSVGPathSegCurvetoCubicAbsConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSSVGPathSegCurvetoCubicAbs* domObject = static_cast<JSSVGPathSegCurvetoCubicAbs*>(asObject(slotBase));
    return JSSVGPathSegCurvetoCubicAbs::getConstructor(exec, domObject->globalObject());
}

void JSSVGPathSegCurvetoCubicAbs::put(ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    ASSERT_GC_OBJECT_INHERITS(this, &s_info);
    lookupPut<JSSVGPathSegCurvetoCubicAbs, Base>(exec, propertyName, value, &JSSVGPathSegCurvetoCubicAbsTable, this, slot);
}

void setJSSVGPathSegCurvetoCubicAbsX(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(thisObject);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    imp->setX(value.toFloat(exec));
}


void setJSSVGPathSegCurvetoCubicAbsY(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(thisObject);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    imp->setY(value.toFloat(exec));
}


void setJSSVGPathSegCurvetoCubicAbsX1(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(thisObject);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    imp->setX1(value.toFloat(exec));
}


void setJSSVGPathSegCurvetoCubicAbsY1(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(thisObject);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    imp->setY1(value.toFloat(exec));
}


void setJSSVGPathSegCurvetoCubicAbsX2(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(thisObject);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    imp->setX2(value.toFloat(exec));
}


void setJSSVGPathSegCurvetoCubicAbsY2(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSSVGPathSegCurvetoCubicAbs* castedThis = static_cast<JSSVGPathSegCurvetoCubicAbs*>(thisObject);
    SVGPathSegCurvetoCubicAbs* imp = static_cast<SVGPathSegCurvetoCubicAbs*>(castedThis->impl());
    imp->setY2(value.toFloat(exec));
}


JSValue JSSVGPathSegCurvetoCubicAbs::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSSVGPathSegCurvetoCubicAbsConstructor>(exec, static_cast<JSDOMGlobalObject*>(globalObject));
}


}

#endif // ENABLE(SVG)