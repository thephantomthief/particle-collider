#include <iostream>
#include <math.h>
#include <float.h>

//No. of identical particles on circumference
int N = 5; //edit this

int particle;

//Coeff. of restitution
float e = 0.8; //edit this

float t; //time

//radius of circle in meters
float R = 100; //edit this
float circumference = 2*3.14159*R; // C = 2 * pi * radius

//To track the position and velocity of each particle. We create position and velocity arrays
//We use polar coordinates; thus, position is measured as an angle only(as radius is constant). 
//Angle measured in Degrees (0 to 360)
float thetaMap[1000]; //edit this if you have more than 1000 particles
float velMap[1000]; //edit this if you have more than 1000 particles

float angle; //angle between positions of two particles

//Counters
int i,j;

//No. of collisions we wish to see
int updates = 100; //edit this

using namespace std;

//Initialize positions as per your wish. Here we initialize them equispaced.
void intializePositions(){

	for(i = 0; i < N; i++){
		thetaMap[i] = i*360.0/N; //edit this
	}
}

//Initialize velocities as per your wish. Here we initialize velocity of each particle equal to its index.
void intializeVelocities(){

	for(i = 0; i < N; i++){
		velMap[i] = (float)i*i; //edit this
	}
}

//Function returns index 'i' of particle which experiences first collision with particle of index 'i+1'.
int findFirstCollision(){

	float min = FLT_MAX;
	float firstParticleToCollide = 0;

	//For each particle we find the time it will take to collide with the particle ahead of it.
	//We update variable min with the lowest time and return the index 'i' of that particle.
	for(i = 0; i < N; i++){

		//If velocity of paricle 'i' is less than or equal to the particle ahead of it, then no collision will occur.
		if(velMap[i%N] <= velMap[(i+1)%N]) continue;

		//Else collision occurs. We need to find the time taken that this collision occurs.
		else{

			//Angle between two consecutive particles
			if(thetaMap[i%N] - thetaMap[(i+1)%N] > 0){
				angle = 360.0 - thetaMap[(i+1)%N] - thetaMap[i%N]; 
			}

			else angle = thetaMap[(i+1)%N] - thetaMap[i%N];

			//              circumference * (angle/360)
			//time taken =  ---------------------------
			//                   Relative velocity
			t = (circumference/360.0)*angle/(velMap[i%N] - velMap[(i+1)%N]);

			//Update min with lowest time and store the index of the associated particle
			if(t < min){
				min = t;
				firstParticleToCollide = i;
			}
		}
	}

	return firstParticleToCollide;
}

//Using the time calculated in findFirstCollision(), we update the positions of all the particles after that time has elapsed
void updatePosition(){
	for(i = 0; i < N; i++){
		thetaMap[i] = thetaMap[i] + (t*velMap[i]*360/circumference);
		
		//To handle rotation. Ex. so that 359 incremented by 5 degrees becomes 4 and 364 degrees
		if(thetaMap[i] > 360.0){
		 	thetaMap[i] = thetaMap[i] - 360.0;
		}
	}
}

//For the particles that collide we update their velocities. Velocities of rest of the partticles remain unchanged.
void updateVelocity(int p1){
	float vA = velMap[p1%N];
	float vB = velMap[(p1+1)%N];

	//Using conservation of momentum
	velMap[(p1+1)%N] = (vA + vB + (e*(vA - vB)))/2;
	velMap[p1%N] = (vA + vB + (e*(vB - vA)))/2;
}

//Function to print velocities of all particles
void printVel(){
	for(i = 0; i < N; i++){
		cout << velMap[i] << '\t';
	}
	cout << endl;
}

int main(){

	//Initialize the system. Time t = 0
	intializePositions();
	intializeVelocities();

	//Each update means a collision.
	for(j = 0; j < updates; j++){

		printVel();
		particle = findFirstCollision(); //Find the first particle to collide
		updateVelocity(particle); //Update velocities of particles that collided
		updatePosition(); //Update postions of all the particles till a collision happens

		//Repeat the loop for another collision
	}

	return 0;
}
