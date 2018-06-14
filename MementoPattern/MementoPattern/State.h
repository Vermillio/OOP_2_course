#ifndef STATE_H
#define STATE_H

#include "common.h"

struct State {
    int brightness;
    int contrast;
    unsigned int saturation;
    string to_json() const {
        return "{ brightness: " + to_string(brightness) + ", contrast: " + to_string(contrast) + ", saturation: " + to_string(saturation) + " }";
    }
    State(int t_brightness, int t_contrast, unsigned int t_saturation) : brightness(t_brightness), contrast(t_contrast), saturation(t_saturation) {};

	bool operator==(const State &s) const { 
		return s.brightness == brightness && s.contrast == contrast && s.saturation == saturation; 
	};
};

#endif