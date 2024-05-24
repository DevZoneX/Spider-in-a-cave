# CaveSpider
This is a programming project for our graphical programming course at Ecole Polytechnique



## Brainstorming
This a cave system with a vibrant ecosystem, with bats, radioactive rabits and spiders.
Each animal can eat some of the others.

The goal is to have great legs animations, crystal lights, a fog, and eating mecanichs.


## Code Structure

The code is structured as following:

 - `main.cpp` : contains the launcher of the code
 - `environment` class : is responsible for handling the environment
 - `scene` class : handles the scene, contains also `gui` class
 - `map` folder :
    - `cave` class : contains the cave structure with all its elements
    - `cave_mesh` class : contains the mesh of the cave boundaries only
 - `entities` folder :
    - `spider` class : contains the code of the spider calculations
 - `subscene` folder: Contains one parent class `subscene` and all test and main scenes subclasses.

 - `utils` folder:
    - `fabric` class : handles the fabric algorithm which calculates the legs position of the spider
    - `key_positions_structure` class : handles the showing of movable spheres
    - `touchable_object` class : handles objects with their collision boxes, is normally a virtual class. This file also contains `collision_partition` class, which is extremely important, it handles the partitionning of collisions accross the level.
    - `collision_object` classes : `collision_object` is a virtual class, and there are then subclasses `collision_sphere` that represents sphere collisions, `collision_box` that represents box collisions...
    - `collision_handler` class : This class is still not clear, will surely be a subclass of `collision_object` in order to manage subpartitionin inside class.
    - `math` class namespace : Has new types like `plane`, `line`, `parallelogram` and `segment` as well as some collision functions.

## Task List

 - Add crystals mesh and class
 - Correct precision problems (float to double maybe)
