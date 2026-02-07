#include "ApplicationHarness.h"

//Include the necessary header file here for your game type
#include "PhysicsScene.h"

int main() {
	PhysicsScene* scene = new PhysicsScene();
	ApplicationHarness program(scene);
	program.Run();
	delete scene;
	return 0;
}