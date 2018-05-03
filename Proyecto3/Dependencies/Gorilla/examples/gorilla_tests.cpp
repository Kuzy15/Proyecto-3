#include <Ogre.h>
 
#include "Gorilla.h"

#include <sstream>

#pragma warning ( disable : 4244 )

/// UNIT TEST SETUP CODE
class UnitTest{public:UnitTest(Gorilla::Silverback* sb, Ogre::Viewport* vp, Ogre::SceneManager* sm):mSilverback(sb),mViewport(vp),
mSceneMgr(sm){}virtual ~UnitTest(){}virtual bool periodic(Ogre::Real){return true;}Gorilla::Silverback* mSilverback;Ogre::Viewport* mViewport;
Ogre::SceneManager* mSceneMgr;};
#define BEGIN_TEST(NAME) class NAME :public UnitTest{public:NAME(Gorilla::Silverback* sb, Ogre::Viewport* vp, Ogre::SceneManager* sm):\
UnitTest(sb,vp,sm){start();}~NAME(){stop();}
#define END_TEST(NAME) };
#define TEST_START bool start()
#define TEST_STOP bool stop()
#define TEST_PERIODIC bool periodic(Ogre::Real timeElapsed)
#define TEST_CONTINUE return true;
#define TEST_FAIL(DESCRIPTION) std::cout << "\n\n\n---\n"DESCRIPTION << "\n---\n\n\n";return false;
/////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// Rectangle Tests
/////////////////////////////////////////////////////////////////////////////////////////

BEGIN_TEST(RectangleTest)
  
  Gorilla::Screen*    mScreen;
  Gorilla::Layer*     mLayer;
  Gorilla::Rectangle* mRectangle;
  
  TEST_START
  {
   mScreen = mSilverback->createScreen(mViewport, "dejavu");
   TEST_CONTINUE
  }

  TEST_STOP
  {
   TEST_CONTINUE
  }

  TEST_PERIODIC
  {
   TEST_CONTINUE
  }

END_TEST(RectangleTest)

/////////////////////////////////////////////////////////////////////////////////////////






















void registerTests(std::map<Ogre::uint, UnitTest*>& tests, Gorilla::Silverback* sb, Ogre::Viewport* vp, Ogre::SceneManager* sm)
{
 tests[0] = new RectangleTest(sb, vp, sm);
}





class App : public Ogre::FrameListener
{
  
 public:
  
  std::map<Ogre::uint, UnitTest*> mTests;
  UnitTest*                       mCurrentTest;
  Gorilla::Silverback*            mSilverback;
  
  App() : mCurrentTest(0)
  {
   
   _makeOgre();

   
   // Create Silverback and load in dejavu
   mSilverback = new Gorilla::Silverback();
   mSilverback->loadAtlas("dejavu");
   
   registerTests(mTests, mSilverback, mViewport, mSceneMgr);
   
  }
  
 ~App()
  {
   delete mSilverback;
   delete mRoot;
  }
  
  void startTest(Ogre::uint index)
  {
  }
  
  void stopTest()
  {
  }
   
  void updateTest(Ogre::Real)
  {
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
   
   mRoot = new Ogre::Root("","");
   mRoot->addFrameListener(this);
   
  #ifdef _DEBUG
   mRoot->loadPlugin("./OgreD/RenderSystem_GL_d.dll");
  #else
   mRoot->loadPlugin("RenderSystem_GL.dll");
  #endif

   mRoot->setRenderSystem(mRoot->getAvailableRenderers()[0]);
   
   Ogre::ResourceGroupManager* rgm = Ogre::ResourceGroupManager::getSingletonPtr();
  	rgm->addResourceLocation("../Media/GUI", "FileSystem");
   
   mRoot->initialise(false);
   
   mWindow = mRoot->createRenderWindow("Gorilla - Press 0 to 9 for unit tests. C to Clear.", 1024, 768, false);
   mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
   mCamera = mSceneMgr->createCamera("Camera");
   mViewport = mWindow->addViewport(mCamera);
   mViewport->setBackgroundColour(Ogre::ColourValue::Black);
   
   rgm->initialiseAllResourceGroups();
  }
  
  
  Ogre::Root*             mRoot;
  Ogre::RenderWindow*     mWindow;
  Ogre::Viewport*         mViewport;
  Ogre::SceneManager*     mSceneMgr;
  Ogre::Camera*           mCamera;
  
};

int main()
{
 App* app  = new App();
 app->mRoot->startRendering();
 delete app;

 return 0;
}


