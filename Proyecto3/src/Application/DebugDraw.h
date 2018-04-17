#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include <Box2D.h>
#include <Ogre.h>

class DebugDraw : public b2Draw
{


private:

	Ogre::SceneManager* mSceneMgr;

public:

	//DebugDraw(){}
	//~DebugDraw(){}

	void setSceneManager(Ogre::SceneManager* sM){ mSceneMgr = sM; }
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	
		Ogre::ManualObject* manual = mSceneMgr->createManualObject("manual");
		manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

		manual->position(-100.0, -100.0, 0.0);  // start position
		manual->position(100.0, -100.0, 0.0);  // draw first line
		manual->position(100.0, 100.0, 0.0);
		manual->position(-100.0, 100.0, 0.0);
		manual->position(-100.0, -100.0, 0.0);  // draw fourth line

		manual->end();
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(manual);

	}



	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {}
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {}
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {}
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {}
	void DrawTransform(const b2Transform& xf) {}
	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color){};
	

	
};

#endif