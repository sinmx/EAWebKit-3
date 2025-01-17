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

#ifndef JSCanvasRenderingContext_h
#define JSCanvasRenderingContext_h

#include "CanvasRenderingContext.h"
#include "JSDOMBinding.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class JSCanvasRenderingContext : public JSDOMWrapper {
public:
    typedef JSDOMWrapper Base;
    static JSCanvasRenderingContext* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<CanvasRenderingContext> impl)
    {
        JSCanvasRenderingContext* ptr = new (JSC::allocateCell<JSCanvasRenderingContext>(globalObject->globalData().heap)) JSCanvasRenderingContext(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    virtual bool getOwnPropertySlot(JSC::ExecState*, const JSC::Identifier& propertyName, JSC::PropertySlot&);
    virtual bool getOwnPropertyDescriptor(JSC::ExecState*, const JSC::Identifier& propertyName, JSC::PropertyDescriptor&);
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    static JSC::JSValue getConstructor(JSC::ExecState*, JSC::JSGlobalObject*);
    virtual void visitChildren(JSC::SlotVisitor&);

    CanvasRenderingContext* impl() const { return m_impl.get(); }

private:
    RefPtr<CanvasRenderingContext> m_impl;
protected:
    JSCanvasRenderingContext(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<CanvasRenderingContext>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::OverridesVisitChildren | Base::StructureFlags;
};

class JSCanvasRenderingContextOwner : public JSC::WeakHandleOwner {
    virtual bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::SlotVisitor&);
    virtual void finalize(JSC::Handle<JSC::Unknown>, void* context);
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld*, CanvasRenderingContext*)
{
    DEFINE_STATIC_LOCAL(JSCanvasRenderingContextOwner, jsCanvasRenderingContextOwner, ());
    return &jsCanvasRenderingContextOwner;
}

inline void* wrapperContext(DOMWrapperWorld* world, CanvasRenderingContext*)
{
    return world;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, CanvasRenderingContext*);
CanvasRenderingContext* toCanvasRenderingContext(JSC::JSValue);

class JSCanvasRenderingContextPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSCanvasRenderingContextPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSCanvasRenderingContextPrototype* ptr = new (JSC::allocateCell<JSCanvasRenderingContextPrototype>(globalData.heap)) JSCanvasRenderingContextPrototype(globalData, globalObject, structure);
        ptr->finishCreation(globalData);
        return ptr;
    }

    static const JSC::ClassInfo s_info;
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

private:
    JSCanvasRenderingContextPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesVisitChildren | Base::StructureFlags;
};

// Attributes

JSC::JSValue jsCanvasRenderingContextCanvas(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsCanvasRenderingContextConstructor(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);

} // namespace WebCore

#endif
