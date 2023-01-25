using namespace std;

#include "Skybox.h"

Skybox::Skybox(Color ntopColor, Color nsideColor, Color nbottomColor, bool noverride, Color noverrideColor, float nintensity, glm::vec3 nup) {
	this->up = nup;
	this->topColor = ntopColor;
	this->sideColor = nsideColor;
	this->bottomColor = nbottomColor;
	this->override = noverride;
	this->overrideColor = noverrideColor;
	this->intensity = nintensity;
}
Color Skybox::getColorAt(glm::vec3 dir) {
	if (this->override)return this->overrideColor;
	float dot = glm::dot(this->up, dir);
	if (dot < 0) {
		return this->sideColor.interpolate(this->bottomColor, abs(dot)).multiply(this->intensity);
	}
	return this->sideColor.interpolate(this->topColor, dot).multiply(this->intensity);
}