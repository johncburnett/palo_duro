/*
 * fracture/src/transform.h
 *
 * fracture
 * Copyright (C) 2016 - epistrata (John Burnett + Sage Jenson)
 * <http://www.epistrata.xyz/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef transform_h
#define transform_h

#include "ofMain.h"
#include "image.h"

//========================================================================
/*
 * Transform is an abstract class that processes one or more images.
 * It serves as a parent class to various other transforms.
 *
 * void draw(void) draws fbo to the screen.
 * void draw_quad(void) maps a texture to a quad.
 */
class Transform {
public:
    ofFbo *fbo;
    BaseImage *input;
    BaseImage *img2;

    // virtual methods
    virtual void process_image(void) =0;
    virtual void update(void) =0;

    // inherited methods
    void draw(void);
    void init_fbo(void);
    ofFbo *get_fbo(void);
    void set_fbo(ofFbo *);
    void draw_quad(void);
};

//========================================================================
/*
 * DisplayImage displays an image with no processing.
 */
class DisplayImage : public virtual Transform {
public:
    DisplayImage(void);
    ~DisplayImage(void);

    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 *  Pan image.
 */

class Pan : public virtual Transform {
public:
    ofShader shader;
    ofImage* img;
    ofFbo* big_fbo;
    ofTexture tex0, tex1;
    float x, y;
    
    Pan(ofImage *big_image);
    ~Pan(void);
    
    void set_corners(float _x, float _y);
    
    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 *  Mirror an image.
 */
class Mirror : public virtual Transform {
public:
    ofShader shader;
    // 0 is left, 1 is right, 2 is top, 3 is bottom
    int mode;
    
    Mirror();
    ~Mirror(void);
    
    void set_mode(int);

    //virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * Smear is a Transform that distorts img1 based on the color of img2
 * Smear(img1, img2, xi, yi, init_dx, init_dy):
 *     input_image : source image
 *     img2 : displacement image
 *     xi : initial x-axis displacement
 *     yi : initial y- axis displacement
 *     init_dx : x-displacement movement
 *     init_dy : y-displacement movement
 *
 * update_delta(new_dx, new_dy) changes dx and dy values.
 *
 */
class Smear : public virtual Transform {
public:
    ofShader shader;
    float x_scale, y_scale;
    float dx, dy;
    BaseImage * fcn;
    int skip, mod; // for cutting resolution

    Smear(BaseImage *, float, float, float, float);
    ~Smear(void);

    void set_scale(float _x, float _y);
    void update_delta(float,float);
    void set_mod(int);

    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * Smear is a Transform that distorts img1 based on the color of img2
 * Smear(img1, img2, xi, yi, init_dx, init_dy):
 *     img1 : source image
 *     img2 : displacement image
 *     xi : initial x-axis displacement
 *     yi : initial y- axis displacement
 *     init_dx : x-displacement movement
 *     init_dy : y-displacement movement
 *
 * update_delta(new_dx, new_dy) changes dx and dy values.
 *
 */
class SmearInner : public virtual Transform {
public:
    ofShader shader;
    float scale;
    BaseImage * fcn;

    SmearInner(BaseImage *);
    ~SmearInner(void);

    void set_scale(float);

    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * Move between video frames.
 */
class FrameMover : public virtual Transform {
public:
    ofShader shader;
    Video* current_frame;
    Still* aggregate;
    int counter;
    float scale;
    
    FrameMover(Video* _video);
    ~FrameMover(void);
    
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * Invert an image.
 */
class Invert : public virtual Transform{
public:
    ofShader shader;
    float scale;
    
    
    Invert(float);
    ~Invert(void);

    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * Make an image grayscale.
 */
class Grayscale : public virtual Transform{
public:
    ofShader shader;
    float scale;
    
    Grayscale(float);
    ~Grayscale(void);

    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * ShadowMask removes pixels that are below a given threshold.
 */
class ShadowMask : public virtual Transform {
public:
    ofShader shader;
    float threshold;
    
    ShadowMask(float);
    ~ShadowMask(void);

    // virtual methods
    void update(void);
    void process_image(void);
};

////========================================================================
///*
// * Colormap maps one image's color to another image. Unfinished.
// */
//class ColorMap : public virtual Transform{
//public:
//    ofShader shader;
//    float scale;
//    ofImage processed;
//
//    ColorMap(Image*, Image*);
//    ~ColorMap(void);
//
//    // virtual methods
//    void update(void);
//    void process_image(void);
//};

//========================================================================
/*
 * Twist distorts a section of the image around a point radially.
 * center is the center of the distortion
 */
class Twirl : public virtual Transform{
public:
    ofShader shader;
    float scale;
    ofVec2f center;
    
    Twirl(void);
    ~Twirl(void);

    void set_center(float, float);
    void set_scale(float);
    
    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * NoiseMask interpolates between two images based on ofImage noise.
 *
 */
class NoiseMask : public virtual Transform {
public:
    ofShader shader;
    float frequency, time, scale;
    BaseImage *mask;
    
    NoiseMask(BaseImage*);
    ~NoiseMask(void);

    void set_scale(float);

    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * HeatDistort uses a depth map to distort an image
 *
 */
class HeatDistort : public virtual Transform {
public:
    ofShader shader;
    int x0, y0;
    BaseImage *mask;
    float frequency;
    float time;
    float distort;
    float rise;

    HeatDistort(BaseImage*, BaseImage*);
    ~HeatDistort(void);

    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
class Aberration : public virtual Transform {
    ofShader shader;
    float amount;
    
    Aberration(void);
    ~Aberration(void);
    
    //virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
class NoiseMaker : public virtual Transform {
    ofShader shader;
    
    NoiseMaker(void);
    ~NoiseMaker(void);

    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
class Swarm : public virtual Transform {
public:
    ofFbo particleFbo;
    ofShader updateShader, drawShader;
    ofVboMesh mesh, quadMesh;
    float opacity;

    // dim of particle location texture
    int w, h;

    Swarm(void);
    ~Swarm(void);

    void createFbo(void);
    void createMesh(void);
    void createPoints(void);
    void setOpacity(float);

    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
class Particle {
public:
    ofVec2f location, original_location;
    ofColor color, bg_color;
    float d;

    Particle(float, float, float, ofColor, ofColor);

    void reset_location(void);
    void up(void);
    void draw_original(void);
    void draw(void);
};

//========================================================================
class Disintegrate : public virtual Transform {
public:
    ofTexture color;
    ofVboMesh mesh, quadMesh;
    vector<Particle*> particles;

    BaseImage *source, *mask, *delta;

    Disintegrate(BaseImage *, BaseImage *, BaseImage *);
    ~Disintegrate(void);

    void create_particles(void);
    bool in_bounds(Particle *);

    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
// Thanks kylemcdonald for this blur¬°¬°!!
float Gaussian(float, float, float);
void GuassianRow(int, vector<float>&, float);
string generateBlurSource(int, float);
string generateCombineSource(int, float);

class ofxBlur : public virtual Transform {
protected:
	ofFbo base;
	vector<ofFbo> ping, pong;

	ofShader blurShader, combineShader;
	float scale, rotation;
	float downsample;
	float brightness;
public:
	ofxBlur(void);
    ~ofxBlur(void);

	void setup(int width, int height, int radius = 10, float shape = .2, int passes = 1, float downsample = .5);

	void setScale(float scale);
	void setRotation(float rotation);
	void setBrightness(float brightness); // only applies to multipass

	void begin();
	void end();
	void draw();
    void draw(ofRectangle rect);
    
    // virtual methods
    void update(void);
    void process_image(void);
    

	ofTexture& getTextureReference();
};

// kylemcdonald end


#endif /* transform_h */
