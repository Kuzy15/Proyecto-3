#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include <Box2D.h>
#include <Ogre.h>
#include <stdio.h>
#include <iostream>
#include "Components.h"

class DebugDraw : public b2Draw
{


private:

	Ogre::SceneManager* mSceneMgr;
	Ogre::ManualObject* mDebug;

public:

	//DebugDraw(){}
	//~DebugDraw(){}

	void setSceneManager(Ogre::SceneManager* sM){
		mSceneMgr = sM; 
		mDebug = mSceneMgr->createManualObject("manual");
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mDebug);

	};

	void clear(){ mDebug->clear(); }

	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {

		mDebug->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
		b2Vec2 firstVertex = vertices[0];
		mDebug->position(firstVertex.x * PPM, firstVertex.y* PPM, 0.0);

		for (int i = 1; i < vertexCount; i++){
			mDebug->position(vertices[i].x * PPM, vertices[i].y * PPM, 0.0 * PPM);
		}
		mDebug->position(firstVertex.x* PPM, firstVertex.y* PPM, 0.0);
		mDebug->end();
	}



	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {

		mDebug->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
		b2Vec2 firstVertex = vertices[0];
		mDebug->position(firstVertex.x * PPM, firstVertex.y* PPM, 0.0);

		for (int i = 1; i < vertexCount; i++){
			mDebug->position(vertices[i].x * PPM, vertices[i].y * PPM, 0.0 * PPM);
		}
		mDebug->position(firstVertex.x* PPM, firstVertex.y* PPM, 0.0);
		mDebug->end();

	}
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {}
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {}
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
	
		mDebug->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

		mDebug->position(p1.x * PPM, p1.y* PPM, 0.0);
		mDebug->position(p2.x* PPM, p2.y* PPM, 0.0);

		mDebug->end();
	
	}
	void DrawTransform(const b2Transform& xf) {}
	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color){
	
		mDebug->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_POINT_LIST);
		mDebug->position(p.x * PPM, p.y* PPM, 0.0);
		mDebug->end();
	
	
	};
	

	
};

#endif