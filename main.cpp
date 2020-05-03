#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Boid.h"

using namespace ci;
using namespace ci::app;

// We'll create a new Cinder Application by deriving from the App class.
class BoidsApp : public App {
public:

    // Cinder will call 'draw' each time the contents of the window need to be redrawn.
    void draw();
    void setup();

private:
    // This will maintain a list of points which we will draw line segments between
    std::vector<Boid> flock;
    vec2 size_;
};

void prepareSettings( BoidsApp::Settings* settings )
{
    settings->setWindowSize( 1000 , 1000 );
}

void BoidsApp::setup()
{

    size_ = vec2(1000,1000);

    for (int i = 0; i < 150; ++i) {
        Boid rand_boid = Boid(rand() % (int)size_.x, rand() % (int)size_.y, size_);
        flock.emplace_back(rand_boid);
    }

}



void BoidsApp::draw()
{

    //float gray = sin( getElapsedSeconds() ) * 0.5f + 0.5f;
    gl::clear( Color( 0, 0, 0 ), true );
    for (int i = 0; i < flock.size(); ++i) {
        flock[i].show();
        flock[i].update();
        flock[i].applyBehaviour(flock);
    }
    if (getWindowSize().x != size_.x or getWindowSize().y != size_.y){
        for (int i = 0; i < flock.size(); ++i) {

            flock[i].windowsize(getWindowSize());
        }
    }

}

// This line tells Cinder to actually create and run the application.
CINDER_APP( BoidsApp, RendererGl, prepareSettings )