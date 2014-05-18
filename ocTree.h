

class ocTree{
	
	posVec centre;         //! The physical center of this node
	posVec halfDepth;  //! Half the width/height/depth of this node
	posVec centreOfMass; // centre of mass of the node
	float heightOfOctant; // height of the node
	bool containsparticles; // wheter it or its children contains particles
	ocTree *children[8]; //! Pointers to child octants
	Particle *particle;   //! particle point to be stored at a node

public:
	float mass; // mass of the node
	ocTree(posVec& _centre, posVec& _halfDepth) // initialiser for octree with centre and half the depth 
		 {
			 centre = _centre; // set centre
			halfDepth = _halfDepth; // set half depth
			particle = NULL; // initially nothing is stored
			
			for(int i=0; i<8; ++i) {
				children[i] = NULL; // initially there are no children
			}
			mass = 0; // set mass to 0
			containsparticles = false; // initially there are no particles within or further down the tree
			centreOfMass = centre; // set centre of mass
			if(halfDepth.y < 0) { // find height of octant
				heightOfOctant = halfDepth.y * -1 * 2; // if number is negative multiply by -1

			} else {

				heightOfOctant = halfDepth.y * 2; 
			}
	}

	ocTree(ocTree& copy)
		 {
			centre = copy.centre; // set centre
			halfDepth = copy.halfDepth; // set halfDepth
			particle = copy.particle; // set particle
	}

	~ocTree() {
		
		for(int i=0; i<8; ++i) {
		
			delete children[i]; // if octree is being destroyed remove its children
		
		}
	}

	
	int getChildWithParticle( posVec point) { // returns octant with particle
		int oct = 0;
		if(point.x >= centre.x) oct |= 4;
		if(point.y >= centre.y) oct |= 2;
		if(point.z >= centre.z) oct |= 1;
		return oct;
	}

	bool isLeaf() { // if there are no children (bottom of tree)

		if (children[0] == NULL) {
			return true;
		} else {
			return false;
		}
	}

	bool containsParticle() { // if this node contains a particle
		if (this->particle == NULL){
			return false;
		} else {
			return true;
		}
	}

	Particle* getParticle()  { // return the nodes particle

		return particle;
	}


	void insert(Particle* _particle) {
		
		if(isLeaf()) { // if were at a leaf
			if(particle==NULL) { // and theres no particle
				particle = _particle; // store the particle
				containsparticles = true;
				return;
			} else {
				
				// currently at bottom of the tree but there is something already stored
				Particle *oldParticle = particle; // hold pointer to the current particle
				particle = NULL; // remove particle

				// Split the current node and create new empty trees for each
				// child octant.
				for(int i=0; i<8; ++i) {
					
					posVec newcentre = centre; // create new centrepoint for octant
					newcentre.x += halfDepth.x * (i & 4 ? .5 : -.5); // find centre using bitwise & conditional ternary operator 
					newcentre.y += halfDepth.y * (i & 2 ? .5 : -.5);
					newcentre.z += halfDepth.z * (i & 1 ? .5 : -.5);
					children[i] = new ocTree(newcentre, halfDepth*.5f); // initialise child at i
				}

				children[getChildWithParticle(oldParticle->getPosition())]->insert(oldParticle); // get octant containing old particle and insert it
				children[getChildWithParticle(oldParticle->getPosition())]->containsparticles = true; // set particles in that octant to true
				children[getChildWithParticle(_particle->getPosition())]->insert(_particle); // insert the inserted particle
				children[getChildWithParticle(_particle->getPosition())]->containsparticles = true; // set particles in that octant to true
				containsparticles = true; // set the particles of the node containing the children to true
			}
		} else {
			
			int octant = getChildWithParticle(_particle->getPosition()); // find octant containing point
			children[octant]->insert(_particle); // insert the point
			children[octant]->containsparticles = true; // set contains particles to true within the child
			containsparticles = true; // set contains particles to true
		}
	}

	float calcdist(posVec v1, posVec v2) // return the distance between two position vectors
	{

		return sqrtf((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y) + (v2.z - v1.z)); // square root of the distances between each component multipled
	}



	void calculateMassAndCentre( ocTree* octree) // finds the mass and centre of mass for an octree with particles inserted
	{
		if(octree->isLeaf()) { // if octree is a leaf
			if(octree->particle!=NULL) { // if the particle stored in the octree is null 
				octree->mass = particle->mass; // set the octrees mass and centre of mass to that of the particles
				octree->centreOfMass = particle->getPosition();
			}

		}
		else // the octree has children 
		{
			for(int i=0; i<8; ++i) { //loop over cchildren
				if (children[i]->isLeaf()){ // if octant at i is a leaf
					if (children[i]->containsParticle()){ // if the child contains a particle
						children[i]->mass = children[i]->getParticle()->mass; // set the childs mass to that of the particle within the child
						children[i]->centreOfMass = children[i]->getParticle()->getPosition(); //set the childs centre of mass to that of the particles
						octree->mass += children[i]->mass; // increment octrees mass by that of the childs
						octree->centreOfMass += children[i]->centreOfMass * children[i]->mass; // increment centre of mass by the childs centre of mass multiplied by its mass
					} else {
						children[i]->mass = NULL; // set mass to null 


					}
				}
				else {

					if (children[i]->containsparticles == true) // if the child or its subsequent children contain particles
						children[i]->calculateMassAndCentre( children[i]);  // calculate the mass and centre of mass for that child and its children
					octree->mass += children[i]->mass;  // the mass of the octree is incremented that of the childs
					octree->centreOfMass += children[i]->centreOfMass * children[i]-> mass; // the octrees centreOfMass is incremented by the childs centre of mass multiplied by its mass
				}



			}
			octree->centreOfMass = octree->centreOfMass / octree->mass; // divide the octrees centre of mass by its mass to finish calculating the com
		}
	}


	void Calculateforce( ocTree* octree, Particle* _particle)
	{
		if( octree->isLeaf() && octree->particle->mass != NULL) 
		{
			_particle->addForce(octree->particle->getPosition(), octree->particle->mass);
		}
		if(! octree->isLeaf())  // the octree is not at a leaf node
		{


			for(int i=0; i<8; ++i) { // loop over children
				if (children[i]->isLeaf() && children[i]->mass != NULL){ // if children at i is a leaf and it doesnt have any mass
					_particle->addForce(children[i]->particle->getPosition(), children[i]->mass); // calculate the force from the particle in the child

				}
				if(!children[i]->isLeaf() && children[i]->mass != NULL) { // if the particle is not a lead and its mass isnt null
					float test = children[i]->heightOfOctant / calcdist(children[i]->centreOfMass, _particle->getPosition()); // height of the octant divided by the distance between the child nodes centre of mass and the particle used to set approximation level
					if ( test < 3 ){ // if its less than one

						_particle->addForce(children[i]->centreOfMass, children[i]->mass); // use the child nodes centre of mass and children to calculate force 

					}
					else {
						for(int j=0; j<8; ++j){ // iterate over childs children
							if(children[i]->children[j]->mass != NULL) { // childs childrens mass isnt null
								
								children[i]->children[j]->Calculateforce(children[i]->children[j], _particle); // calculate force of childs child
							}
						}
					}					
				}
			}
		}
	}

};





