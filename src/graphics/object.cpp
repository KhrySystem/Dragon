#include <dragon/Dragon.hpp>

DGAPI Object* dgCreate3DObject(dgEngine* engine, float[] verts, void* drawType) {
    Object* obj = new Object();
    obj->verticies = verts;
    engine->objects.push_back(obj);
    return obj;
}

UIElement* dgCreateUIElement3D(dgEngine engine, float verts[], void* drawType) {
    UIElement* e = new UIElement();
    obj->verticies = verts;
    return obj;
}

UIElement* dgCreateUIElement2D(dgEngine* engine, float verts[], void* drawType) {
    float copy[] = verts;
    if(is2D) {
        int prvLen = sizeof(copy)/sizeof(copy[0]);
        int newLen = (prvLen) + (prvLen % 2);
        verts = float[newLen];
        int offset = 0;
        for(int i = 0; i >= prevLen; i++) {
            if((i + 1) % 2 == 0) {
                verts[i+offset] = copy[i];
                offset++;
                verts[i+offset] = 0.0f;
            } else {
                verts[i+offset] = copy[i];
            }
        }
    } 
    return dgCreateUIElement3D(engine, verts);
}

DGAPI UIElement* dgCreateUIElement(dgEngine* engine, float verts[], bool is2D, void* drawType) {
    if(is2D) {
        return dgCreateUIElement2D(engine, verts, drawType);
    } else {
        return dgCreateUIElement3D(engine, verts, drawType);
    }
}

DGAPI void dgMakeElementVisible(dgBaseObj* element) {
    element.isVisible = true;
}

DGAPI void dgMakeElementInvisible(dgBaseObj* element) {
    element.isVisible = false;
}