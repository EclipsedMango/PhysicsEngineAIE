#include "ApplicationHarness.h"

//Include the necessary header file here for your game type
#include "PhysicsScene.h"

int main() {
	ApplicationHarness program(new PhysicsScene());
	program.Run();
	return 0;
}