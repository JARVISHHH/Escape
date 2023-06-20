# Escape

Hello, this is Escape, a dungeon escape rogue-like game created by a self-made 3D game engine.

Check out this slides to have an insight about this game (and some advanced engine features): [Escape 3D game engine](https://docs.google.com/presentation/d/1uaBW2uZxgA3INfNzQe-RBvGKkVebQzpuPwg3CGzBgKc/edit#slide=id.p)

## How to run

For Visual Studio, open the folder and run the `CMakeList.txt`.

For QtCreator, open the `CMakeList.txt` and run.

## Demo



## Engine Feature

Most features are done on function layer. Rendering is done on OpenGL.

### GameObject Structure

Typical World / System / GameObject / Component hierarchy

### Collision

#### Basic Algorithm

Dynamic game objects: using cylinder collision.

Static game objects (environments): using Ellipsoid-Triangle collision.

#### Optimizations

Dynamic game objects: optimized using BVH.

Static game objects (environments): optimized using Hierarchical Grid.

### AI

Pathfinding: Use A* algorithm to generate a path based on navigation mesh.

Decision making: Behavior trees.

### Graphics

Particle systems

Bump mapping

Shadows for spot lights and point lights

### UI

Basic UI toolkit, including buttons, images and text.



## The Game

### Goal

The goal of Escape is to find the escape room and get as higher scores as possible.

The character dies when the time is out or its hp drop down to 0.

### Character

Character can shoot bullets to kill enemies.

### Enemy

There are 2 types of enemies, chasing enemy and shooting enemy.

The chasing enemy moves on the ground and follows the character.

The shooting enemy will shoot bullets to the character.

### Rooms

There are 4 types of rooms currently: safe room, spikes room, enemy room and pick-up room.

### Pick-ups

There are 3 types of pick-ups that may show up in the game. Players can try to get these pick-ups and find out their functionality.
