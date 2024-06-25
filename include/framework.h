#ifndef FRAMEWORK_H
#define FRAMEWORK_H

// Raylib
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <misc.hpp>

// Component/entity/scene
#include <entity.hpp>
#include <component.hpp>
#include <scene.hpp>

// Components
#include <health_component.hpp>
#include <transform_component.hpp>
#include <camera_component.hpp>
#include <collider_component.hpp>
#include <area_component.hpp>
#include <animation_component.hpp>

// Objects
#include <objects/drawables.hpp>
#include <objects/sprites.hpp>
#include <objects/trails.hpp>
#include <objects/particles.hpp>
#include <objects/audio.hpp>

// Entities
#include <test_entity.hpp>
#include <tilemap_entity.hpp>

// Scenes
#include <scenes/game.hpp>

#endif