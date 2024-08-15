#ifndef RENDERER_H
#define RENDERER_H

#include "../Entities/gameObject.h"
#include "../Entities/scene.h"

void initRenderer(Scene& scene);
void RenderFrame(GameObject& gameObject);
void RenderScene(Scene& scene);

#endif // RENDERER_H
