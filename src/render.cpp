/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Glesly: my GLES-based rendering library
 * Purpose:     Class to execute one OpenGL program with Objects
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <GLES2/gl2.h>

#include <glesly/object.h>

#include "render.h"

using namespace Glesly;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *       Class Render:                                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Render::Render(CameraMatrix & camera, float aspect):
    myScreenAspect(aspect),
    myCameraMatrix(*this, "camera_matrix", camera),
    objInitList(nullptr),
    freeObjIniters(nullptr)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 for (objectIniter * oi = initers; oi < initers + initerSize; ++oi) {
    oi->next = freeObjIniters;
    freeObjIniters = oi;
    SYS_DEBUG(DL_INFO3, "ObjIniter: " << oi << ", next: " << oi->next);
 }
}

Render::~Render()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

void Render::Cleanup(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 GetObjectList().Cleanup();
}

void Render::InitGLObject(ObjectWeak & object)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 Threads::Lock _l(myObjInitMutex);

 objectIniter * oi = freeObjIniters;

 ASSERT(oi, "too few Object Initializers allocated");
 SYS_DEBUG(DL_INFO3, "ObjIniter: " << oi << ", next: " << oi->next);

 freeObjIniters = oi->next; // get it from the free list

 oi->object = object;       // assign the object

 oi->next = objInitList;    // put it in the initializer list
 objInitList = oi;
}

ObjectPtr Render::GetObject2Init(void)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 Threads::Lock _l(myObjInitMutex);

 ObjectPtr op;

 do {
    objectIniter * oi = objInitList;

    if (!oi) {
        return ObjectPtr();
    }

    objInitList = oi->next;

    oi->next = freeObjIniters;
    freeObjIniters = oi;

    // Try to get the referenced object:
    op = oi->object.lock();

    // The object referenced by this entry may have been deleted: try the next one in this case.

 } while (!op);

 return op;
}

void Render::NextFrame(const SYS::TimeDelay & frame_start_time)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 BeforeFrame();

 UseProgram();

 InitGLVariables();
 ActivateVariables();

 Frame(frame_start_time);

 for (;;) {
    ObjectPtr obj = GetObject2Init();
    if (!obj) {
        break;
    }
    obj->initGL();
 }

 ObjectListPtr p = GetObjectListPtr(); // The pointer is copied here to solve thread safety

 if (p) {
    for (ObjectListIterator i = p->begin(); i != p->end(); ) {
        ObjectListIterator j = i++;
        ObjectPtr obj = *j;
        if (obj->toBeDeleted) {
            p->erase(j);
            obj->uninitGL();
        } else {
            obj->DrawFrame(frame_start_time);
        }
    }
 }

 UnuseProgram();

 AfterFrame();
}

void Render::MouseClickRaw(int x, int y, int index, int count)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (IsInputBlocked()) {
    return;
 }

 float horiz = x;
 float vert = y;

 try {
    ConvertMouseCoordinates(horiz, vert);
 } catch(::EX::Assert & ex) {
    DEBUG_OUT("Cannot calculate mouse position because " << ex.what());
    return;
 }

 SYS_DEBUG(DL_INFO2, "Mouse button #" << index << " click #" << count << ", at " << x << "x" << y << " -> " << horiz << "x" << vert);

 ObjectListPtr p = GetObjectListPtr(); // The pointer is copied here to solve thread safety

 if (p) {
    SYS_DEBUG(DL_INFO2, "Having " << p->size() << " objects");
    for (ObjectListIterator i = p->begin(); i != p->end(); ++i) {
       if ((*i)->MouseClick(horiz, vert, index, count)) {
           break;
       }
    }
 }
}

void Render::KeyboardClick(UTF8::WChar key)
{
 SYS_DEBUG_MEMBER(DM_GLESLY);

 if (IsInputBlocked()) {
    return;
 }

 ObjectListPtr p = GetObjectListPtr(); // The pointer is copied here to solve thread safety

 if (p) {
    SYS_DEBUG(DL_INFO2, "Having " << p->size() << " objects");
    for (ObjectListIterator i = p->begin(); i != p->end(); ++i) {
       (*i)->KeyboardClick(key);
    }
 }
}

int Render::GetCallbackTimeLimit(void) const
{
 return 30; // TODO: be configurable!
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *       Class Render3D:                                                                 *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Render3D::Render3D(RenderInfo & renderInfo):
    Render(renderInfo.myCamera),
    myRenderInfo(renderInfo),
    myT1Matrix(*this, "t0_matrix", renderInfo.myTransform[0]),
    myT2Matrix(*this, "t1_matrix", renderInfo.myTransform[1]),
    myT3Matrix(*this, "t2_matrix", renderInfo.myTransform[2]),
    myT4Matrix(*this, "t3_matrix", renderInfo.myTransform[3])
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

Render3D::~Render3D()
{
 SYS_DEBUG_MEMBER(DM_GLESLY);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
