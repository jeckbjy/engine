#pragma once
#include "Object.h"
#include "Rect.h"

CU_NS_BEGIN

class Surface;
class Window;
class Scene;
class Camera;
class Canvas;
class RenderPath;

class CU_API View
{
public:
	View();
	~View();

	void update();
	void render();

	void addViewport(const RectI& rect, Scene* scene = NULL, Camera* camera = NULL);
	Scene* getScene(size_t viewport = 0);
	Camera* getCamera(size_t viewport = 0);

};

CU_NS_END
