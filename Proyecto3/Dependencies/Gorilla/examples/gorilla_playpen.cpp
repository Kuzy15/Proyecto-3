#include <Ogre.h>
#include "Gorilla.h"
#include <iostream>

#include <sstream>

#pragma warning ( disable : 4244 )

class App : public Ogre::FrameListener
{
  
 public:
  
  Ogre::Real              mTimer, mTimer2;
  Gorilla::Silverback*    mSilverback;
  Gorilla::Screen*        mScreen;
  Gorilla::Layer*         mLayer;
  
  Gorilla::Polygon* poly;
  Gorilla::LineList*       list;
  Gorilla::Caption*        caption;
  Gorilla::Rectangle*      rect;
  Gorilla::QuadList*       quads;
  Gorilla::MarkupText*     markup;
  
  App() : mTimer(0), mTimer2(0), mNextUpdate(0)
  {
   
   _makeOgre();

  // Create Silverback and load in dejavu
  mSilverback = new Gorilla::Silverback();
  mSilverback->loadAtlas("dejavu");
  mScreen = mSilverback->createScreen(mViewport, "dejavu");
  mScreen->setOrientation(Ogre::OR_DEGREE_270);
  Ogre::Real vpW = mScreen->getWidth(), vpH = mScreen->getHeight();

  // Create our drawing layer
  mLayer = mScreen->createLayer(0);
  rect = mLayer->createRectangle(0,0, vpW, vpH);
  rect->background_gradient(Gorilla::Gradient_Diagonal, Gorilla::rgb(98,0,63), Gorilla::rgb(255,180,174));
  
  markup = mLayer->createMarkupText(9,5,5, "%@24%A Haiku\n%@14%Written by Betajaen%@9%\nSo many to choose from\nPretty typefaces on Ogre screen\nTime to update Git");
  
  caption = mLayer->createCaption(9, vpW - 55, 5, "9");
  caption->width(50);
  caption->align(Gorilla::TextAlign_Right);

  caption = mLayer->createCaption(14, vpW - 55, 18, "14");
  caption->width(50);
  caption->align(Gorilla::TextAlign_Right);

  caption = mLayer->createCaption(24, vpW - 55, 33, "24");
  caption->width(50);
  caption->align(Gorilla::TextAlign_Right);
  }
  
 ~App()
  {
   std::cout << "\n** Average FPS is " << mWindow->getAverageFPS() << "\n\n";
   delete mSilverback;
   delete mRoot;
  }
  
  bool frameStarted(const Ogre::FrameEvent& evt)
  {
   
   if (mWindow->isClosed())
    return false;
   
   return true;
  }
  
  
  void  _makeOgre()
  {
   srand(time(0));
   
   
   Ogre::String resCfgLoc;
   Ogre::String plugCfgLoc;
   Ogre::ConfigFile cf;

   resCfgLoc = "OgreD/resources_d.cfg";
   plugCfgLoc = "OgreD/plugins_d.cfg";

   
   mRoot = new Ogre::Root(plugCfgLoc);
   mRoot->addFrameListener(this);
   //name: Path to resources in disk,
   //loctype: defines what kind of location the element is (e.g. Filesystem, zip..)
   Ogre::String name, locType;

   //We now iterate through rach section in the resources.cfg.
   //Sections are signaled as [NAME]
   Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
   while (secIt.hasMoreElements())
   {
	   Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
	   Ogre::ConfigFile::SettingsMultiMap::iterator it;

	   //Now we are iterating INSIDE the section [secIt]
	   for (it = settings->begin(); it != settings->end(); ++it)
	   {
		   locType = it->first;
		   name = it->second;

		   //We now know the type of the element and its path.
		   //We add it as a location to the Resource Group Manager
		   Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);


	   }
   }
   
	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
		return;

   
   mWindow = mRoot->initialise(true, "JEJ");
   mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
   mCamera = mSceneMgr->createCamera("Camera");
   mViewport = mWindow->addViewport(mCamera);
   mViewport->setBackgroundColour(Gorilla::webcolour(Gorilla::Colours::FireBrick));
   
   Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  }
  


  
  Ogre::Root*             mRoot;
  Ogre::RenderWindow*     mWindow;
  Ogre::Viewport*         mViewport;
  Ogre::SceneManager*     mSceneMgr;
  Ogre::Camera*           mCamera;
  Ogre::Real              mNextUpdate;
  
};

int main()
{
 App* app  = new App();
 app->mRoot->startRendering();
 delete app;
 return 0;
}


