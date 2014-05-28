
struct posVec { 
	
		struct {
			
			float x; // float to hold x  co ord
			float y; // float to hold y co ord
			float z; // float to hold x co ord
				
	};

	posVec() { }


	posVec(float _x, float _y, float _z){ 
	 x = _x; // initialise a posVec with passed in values
	 y = _y;
	 z = _z;
	
	}

	posVec operator + (posVec vec) {
		
		return posVec(x+vec.x,  y+vec.y, z+vec.z); // add x y and z components
	
	}

		posVec operator / (float n) {
		
			return posVec(x/n, y/n, z/n); // divide components by float n
	}


	posVec operator += (posVec vec) {
	
		x += vec.x; // increment components from vec's components
		y += vec.y;
		z += vec.z;
		return *this; // return this posvec

	}
	posVec operator - (posVec vec) {
		
		return posVec(x-vec.x, y-vec.y, z-vec.z);  // subtract components from vec's
	
	}

		posVec operator * (float n) {
	
			return posVec(x*n, y*n, z*n); // multiply components by n
	
		}

		posVec operator * (posVec vec)  {
		
			return posVec(x*vec.x, y*vec.y, z*vec.z); // multiply by vecs relecant components
	}


};


