
GLfloat colour[] = {1, 0, 0};

class Particle {
	posVec position;

public:
	float xvelocity; // x movement
	float yvelocity; // y movement
	float zvelocity; // z movement
	float mass; // particles mass
	float xforce; // force acting in x direction
	float yforce; // force acting in y direction
	float zforce; // force acting in z direction

	Particle() { 
		xforce = 0;
		yforce = 0;
		zforce = 0;
		xvelocity = 0
		yvelocity = 0;
		zvelocity = 0;
		//mass = sqrtf(rand()); // sets a random mass
		mass = 5;
	}
	Particle( posVec position) {
		this->position = position; 
		xvelocity = 0;
		yvelocity = 0;
		zvelocity = 0;
		//mass = sqrtf(rand());
		mass = sqrtf(rand());
		xforce = 0;
		yforce = 0;
		zforce = 0;


	}
	posVec getPosition()  { // returns the position vector
	
		return position; 
	
	}
	
	 void setPosition(posVec p) {  // sets position vector
		position = p; 
	}

	void draw() { // opengl draw

		glPushMatrix(); // push the matrix
		glTranslatef(position.x, position.y, position.z); // move to position of particle

		glColor4fv(colour); // set colour


		glutSolidSphere(15, 25.0, 25.0); // draw a solid sphere


		glPopMatrix(); // pop the matrix

	}

	void addForce(Particle* p) { // adds the force for particle to particle interaction


		float dx = p->position.x - position.x; // find distance in x direction
		float dy = p->position.y - position.y; // find distance in y direction
		float dz = p->position.z - position.z; // find distance in z direction



		float distanceSquared = dx * dx + dy * dy + dz * dz + 15; // squared distance 25 added as a softener to stop infinite forces
		float distCube = distanceSquared * distanceSquared * distanceSquared; // cube the distance squared
		float invDistCube = 1/sqrtf(distCube); 

		float s = p->mass * invDistCube; // magnitude

		xforce += dx * s; // calculates and updates forces
		yforce += dy * s;// calculates and updates y force
		zforce += dz * s;// calculates and updates zforce

	}

	void addForce(posVec com, float _mass) { // adds force for approximate barnes hut


		float dx = com.x - position.x; // works like above function
		float dy = com.y - position.y;
		float dz = com.z - position.z;



		float distSqr = dx * dx + dy * dy + dz * dz + 15;
		float distSixth = distSqr * distSqr * distSqr;
		float invDistCube = 1/sqrtf(distSixth);

		float s = _mass * invDistCube;

		xforce += dx * s;
		yforce += dy * s;
		zforce += dz * s;

	}

	void updatePosition(float timeStep) {
		
		xvelocity += timeStep * xforce; // calculates velocity in x
		yvelocity += timeStep * yforce;// ^^ same in y
		zvelocity += timeStep * zforce;// ^^ same in z
		
		position.x += xvelocity; // update position vectors x value
		position.y += yvelocity; // update position vectors y value
		position.z += zvelocity;// update position vectors z value
	}



};


