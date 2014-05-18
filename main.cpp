#include <vector>
#include <glut.h>
#include <stdlib.h>
#include<iostream>
#include "posVec.h"
#include "Particle.h"
#include "ocTree.h"


std::vector<posVec> positions;

Particle *Particles;
int numberOfFrames;
float currentTime;
float previousTime;
float fps;
ocTree *octree;
int npositions;


void draw() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -8000.0);      
	for(int i = 0; i < positions.size(); i++) {

		Particles[i].draw();
	}

	glutSwapBuffers();	

}

float randLimited(){											// Random number generation

	return rand()%(500-1)+1; 
}

posVec randPosVec(){ // Random vector

	return posVec(randLimited(), randLimited(), randLimited()); 

}

void initialise() {
	// Create an Octree centered at 250, 250, 250
	// with physical dimension 10000x10000x10000
	octree = new ocTree(posVec(250,250,250), posVec(5000,5000,5000));

	// Create 200 positions vectors
	npositions = 300;
	for(int i=0; i<npositions; ++i) {
		positions.push_back(randPosVec());
	}

	Particles = new Particle[npositions]; // array to hold particles initialised to the size of number of positions/particles
	for (int i=0; i<npositions; ++i) { // loop over number of positions
		Particles[i].setPosition(positions[i]); // set position of particle at i
		octree->insert(&Particles[i]); // insert particle in to oct tree
	}	

	
}

void calculateFPS()
{

	
	currentTime = glutGet(GLUT_ELAPSED_TIME); // Number of milliseconds since glutInit called (or first call to glutGet(GLUT_ELAPSED_TIME)). http://www.opengl.org/resources/libraries/glut/spec3/node70.html
	numberOfFrames++; // increment number of frames
	int timePassed = currentTime - previousTime; //  Calculate time passed

	if(timePassed > 1000) // if the time passed is a second or more
	{
		
		fps = numberOfFrames / (timePassed / 1000); //  calculate the number of frames per second
		previousTime = currentTime; //  Set time for next calculation
		numberOfFrames = 0; //  Reset frame count
		std::cout << fps << ".\n";

	}
}

void particleSolve() {
	
	for(int i = 0; i < npositions; i++) { // loop over particles
		Particles[i].xforce = 0; // zero particles forces
		Particles[i].yforce = 0;
		Particles[i].zforce = 0;
		for(int j = 0; j < npositions; j++) { // loops over all other particles  2 loops n2 complexity
			if ( i != j) { // if particles arent the same
				Particles[i].addForce(&Particles[j]); // add their forces
			}

		}
		Particles[i].updatePosition(0.05); // update position 0.05 timestep
	}


}

void bhSolve() {
	octree->calculateMassAndCentre(octree);  // find mass and centre of mass of octree
	
	for(int i = 0; i < npositions; i++) { // loops over particles
		Particles[i].xforce = 0; // zero the particles forces
		Particles[i].yforce = 0;
		Particles[i].zforce = 0;
		octree->Calculateforce(octree, &Particles[i]); // calculate the force acting on the particle from all others
		Particles[i].updatePosition(0.06); // update position
	}
	octree->~ocTree(); // call destructor or octree to clean up
	octree = new ocTree(posVec(250,250,250), posVec(5000,5000,5000)); // initialise a new octree as octree
	for(int i = 0; i < npositions; i++) { // loops over the particles
		octree->insert(&Particles[i]); // reinsert to octree
	}
}	

void idle(){ // idle method for opengl
	calculateFPS(); 
	//particleSolve();
	bhSolve();
	glutPostRedisplay(); // calls a redisplay for opengl

}

int main(int argc, char** argv) {

	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { -1.0, -1.0, -1.0, 0.0 };
	initialise(); // call initialiser
	glutInit(&argc, argv); // initialise glut
	glEnable( GL_DEPTH_TEST );  // enable depth
	glutInitDisplayMode(GLUT_DOUBLE ); // double buffering  
	glutInitWindowSize(1200, 800); // initialise a window 1200 x 800
	glutCreateWindow("Nbody"); // create a window
	glClearColor(0.0, 0.0, 0.0, 1.0); // set bg colour
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); // set lighting
	glEnable(GL_LIGHTING); // enable lighting
	glEnable(GL_LIGHT0);   // enable light                              
	glMatrixMode(GL_PROJECTION); //load the projection matrix


	gluPerspective(45.0, 1200/800, 200, 9000); // set the perspection

	
	glutDisplayFunc(draw); // set draw as display function
	glutIdleFunc(idle); // set idle as idle function
	glutMainLoop(); // enter gluts main loop

}

