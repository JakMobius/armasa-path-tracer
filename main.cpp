
#include "examples/tank/tank_example_view.hpp"
#include "examples/cornell_box/cornell_box_example_view.hpp"
#include "examples/cornell_box_sphere/cornell_box_sphere_example_view.hpp"
#include "examples/random_spheres/random_spheres_example_view.hpp"
#include "examples/playground/playground_view.hpp"

int main(){
    // Uncomment any example you want to see
    // Be sure to read warnings in README before running.

//    RandomSpheresExampleView view;
//     CornellBoxSphereExampleView view;
    // CornellBoxExampleView view;
     TankExampleView view;
//    PlaygroundView view;

    view.init();
    view.loop();
}