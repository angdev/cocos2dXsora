// Ŭnicode please
#include "stdafx.h"
#include "aura_layer.h"

USING_NS_CC;

AuraLayer::AuraLayer(GameWorld *world) : world_(world) {

}

AuraLayer::~AuraLayer() {

}

bool AuraLayer::init() {
    if(!CCLayer::init())
        return false;

    return true;
}

void AuraLayer::draw() {

}
