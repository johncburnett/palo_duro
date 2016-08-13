/*
 * fracture/src/kernel.cpp
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "main.h"

//========================================================================
Stream::Stream(void) {
    num_nodes = 0;
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
}

void Stream::add_transform(Transform *T) {
    nodes.push_back(node());
    nodes[num_nodes].transform = T;
    num_nodes++;
}

void Stream::evaluate(void) {
    nodes[0].transform->update();
    fbo = nodes[0].transform->fbo;
    for(int i = 1; i < num_nodes; i++) {
        nodes[i].transform->img1 = new Image(fbo);
        nodes[i].transform->update();
        fbo = nodes[i].transform->fbo;
    }
}

void Stream::draw(void) {
    fbo.draw(0, 0);
}

//========================================================================
Kernel::Kernel(void) {
    current_frame = 0;
    num_frames = 1;
    start_time = 0.0f;
    target_time = 5.0f;
    loop = false;
    frames.push_back(frame());
    frames[0].t = 5.0f;
}

void Kernel::add_stream(Stream *S, int frame_index) {
    frames[frame_index].streams.push_back(S);
    frames[frame_index].n++;
}

void Kernel::add_frame(float l) {
    frames.push_back(frame());
    frames[num_frames].t = l;
    num_frames++;
}

ofFbo Kernel::get_stream_fbo(int frame_index, int stream_index) {
    return frames[current_frame].streams[stream_index]->fbo;
}

ofFbo Kernel::get_frame_fbo(int frame_index) {
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
    fbo.begin();
    ofClear(0, 0, 0, 1);
    draw();
    fbo.end();
    return fbo;
}

void Kernel::set_frame_length(int frame_index, float l) {
    frames[frame_index].t = l;
}

void Kernel::toggle_loop(bool val) {
    loop = val;
}

void Kernel::update(void) {
    float time = ofGetElapsedTimef();
    
    if(loop) {
        if(time - start_time >= target_time) {
            current_frame = (current_frame + 1) % num_frames;
            start_time = time;
            target_time = frames[current_frame].t;
        }
    } else {
        if((time - start_time >= target_time) && current_frame < num_frames-1) {
            current_frame++;
            start_time = time;
            target_time = frames[current_frame].t;
        }
    }
    
    for(int i = 0; i < frames[current_frame].n; i++) {
        frames[current_frame].streams[i]->evaluate();
    }
}

void Kernel::draw(void) {
    for(int i = 0; i < frames[current_frame].n; i++) {
        frames[current_frame].streams[i]->draw();
    }
}