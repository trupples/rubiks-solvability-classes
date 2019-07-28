#include<utility> // std::pair

/*
 * Looking at the cube from the top with F2L solved, this is the edge and corner
 * numbering (eX is edge X, cX is corner X).
 *
 * +----+----+----+
 * | c1 | e1 | c0 |
 * +----+----+----+
 * | e2 |    | e0 |
 * +----+----+----+
 * | c2 | e3 | c3 |
 * +----+----+----+
 *
 * Each edge can be flipped or not (whether yellow is facing up or not).
 * Each corner can be rotated to one of 3 positions determined by what direction
 * the yellow sticker is facing. Facing up is rotation 0. 1 and 2 are counter
 * clockwise rotations of rotation 0.
 *
 * edge/cornerStates[i] describes what cubie is currently on that position, not
 * where the cubie that initially was there is now.
 *
 */

#define color first
#define rotation second

class Cube
{
public:
	std::pair<int, int> edgeStates[4];
	std::pair<int, int> cornerStates[4];

	// Default constructor is the standard solved face in class 0
	Cube() :
		edgeStates({{0,0}, {1,0}, {2,0}, {3,0}}),
		cornerStates({{0,0}, {1,0}, {2,0}, {3,0}}) {}

	// Copy constructor ez
	Cube(const Cube &copy) :
		edgeStates(copy.edgeStates),
		cornerStates(copy.cornerStates) {}

	// Hash output isn't compact and contains some redundancy.
	int hash() const
	{
		// copy so we don't mutate this during the reorientation
		Cube cube(*this);

		while(cube.edgeStates[0].color != 0)
		{
			cube.RotateClockwise();
		}

		int hash = 0;

		hash = hash * 2 + cube.edgeStates[0].rotation;
		
		hash = hash * 2 + cube.edgeStates[1].rotation;
		hash = hash * 4 + cube.edgeStates[1].color;
		
		hash = hash * 2 + cube.edgeStates[2].rotation;
		hash = hash * 4 + cube.edgeStates[2].color;
		
		hash = hash * 2 + cube.edgeStates[3].rotation;
		hash = hash * 4 + cube.edgeStates[3].color;

		hash = hash * 3 + cube.cornerStates[0].rotation;
		hash = hash * 4 + cube.cornerStates[0].color;

		hash = hash * 3 + cube.cornerStates[1].rotation;
		hash = hash * 4 + cube.cornerStates[1].color;

		hash = hash * 3 + cube.cornerStates[2].rotation;
		hash = hash * 4 + cube.cornerStates[2].color;

		hash = hash * 3 + cube.cornerStates[3].rotation;
		hash = hash * 4 + cube.cornerStates[3].color;	

		return hash;
	}

	inline void flipEdge(std::pair<int,int> &edge)
	{
		edge.rotation ^= 1;
	}

	inline void rotateCorner(std::pair<int, int> &corner, int third_turns)
	{
		corner.rotation = (corner.rotation + third_turns) % 3;
	}

	// Futeres encode 3 values, namely whether to:
	// * flip e0
	// * swap e0 and e1
	// * rotate c0 by 0, 1, or 2
	// encoded like (c0_rot + 3*flip_e0 + 6*swap_e0_e1)
	void applyFutere(int futere)
	{
		const int swap_e0_e1 = futere/6 % 2,
			flip_e0 = futere/3 % 2,
			rot_c0 = futere % 3;

		if(swap_e0_e1) swap(edgeStates[0], edgeStates[1]);
		if(flip_e0) flipEdge(edgeStates[0]);
		rotateCorner(cornerStates[0], rot_c0);
	}

	void RotateClockwise()
	{
		std::pair<int,int> aux=edgeStates[0];
		edgeStates[0]=edgeStates[1];
		edgeStates[1]=edgeStates[2];
		edgeStates[2]=edgeStates[3];
		edgeStates[3]=aux;

		aux=cornerStates[0];
		cornerStates[0]=cornerStates[1];
		cornerStates[1]=cornerStates[2];
		cornerStates[2]=cornerStates[3];
		cornerStates[3]=aux;
	}

	void RotateAntiClockwise()
	{
		std::pair<int,int> aux=edgeStates[0];
		edgeStates[0]=edgeStates[3];
		edgeStates[3]=edgeStates[2];
		edgeStates[2]=edgeStates[1];
		edgeStates[1]=aux;

		aux=cornerStates[0];
		cornerStates[0]=cornerStates[3];
		cornerStates[3]=cornerStates[2];
		cornerStates[2]=cornerStates[1];
		cornerStates[1]=aux;
	}

	void OrientEdges()
	{
		flipEdge(edgeStates[1]);
		flipEdge(edgeStates[3]);

		std::pair<int,int> aux=edgeStates[0];
		edgeStates[0]=edgeStates[3];
		edgeStates[3]=edgeStates[1];
		edgeStates[1]=aux;

		rotateCorner(cornerStates[1],1);
		rotateCorner(cornerStates[2],2);

		swap(cornerStates[0],cornerStates[1]);
		swap(cornerStates[2],cornerStates[3]);
	}

	void PLLUaPerm()
	{
		std::pair<int,int> aux = edgeStates[0];
		edgeStates[0] = edgeStates[3];
		edgeStates[3] = edgeStates[2];
		edgeStates[2] = aux;
	}

	void PLLUbPerm()
	{
		std::pair<int,int> aux = edgeStates[0];
		edgeStates[0] = edgeStates[2];
		edgeStates[2] = edgeStates[3];
		edgeStates[3] = aux;
	}

	void PLLZPerm()
	{
		swap(edgeStates[0],edgeStates[1]);
		swap(edgeStates[2],edgeStates[3]);
	}

	void PLLHPerm()
	{
		swap(edgeStates[0],edgeStates[2]);
		swap(edgeStates[1],edgeStates[3]);
	}

	void PLLAaPerm()
	{
		std::pair<int,int> aux=cornerStates[0];
		cornerStates[0]=cornerStates[1];
		cornerStates[1]=cornerStates[3];
		cornerStates[3]=aux;
	}

	void PLLEPerm()
	{
		swap(cornerStates[0],cornerStates[3]);
		swap(cornerStates[1],cornerStates[2]);
	}

	void OLLSune()
	{
		PLLUaPerm();

		rotateCorner(cornerStates[3],2);
		rotateCorner(cornerStates[1],2);
		rotateCorner(cornerStates[2],2);

		swap(cornerStates[0],cornerStates[2]);
		swap(cornerStates[1],cornerStates[3]);
	}

	void OLLAntiSune()
	{
		PLLUbPerm();

		rotateCorner(cornerStates[0],1);
		rotateCorner(cornerStates[2],1);
		rotateCorner(cornerStates[3],1);

		swap(cornerStates[0],cornerStates[2]);
		swap(cornerStates[1],cornerStates[3]);
	}

	void OLLCross()
	{
		RotateClockwise();
		RotateClockwise();
		PLLUaPerm();
		RotateClockwise();
		RotateClockwise();

		rotateCorner(cornerStates[0],1);
		rotateCorner(cornerStates[1],2);
		rotateCorner(cornerStates[2],1);
		rotateCorner(cornerStates[3],2);
	}

	void OLLDeadGuy()
	{
		rotateCorner(cornerStates[0],1);
		rotateCorner(cornerStates[1],1);
		rotateCorner(cornerStates[2],2);
		rotateCorner(cornerStates[3],2);

		swap(cornerStates[0], cornerStates[2]);
		swap(cornerStates[1], cornerStates[3]);

		std::pair<int,int> aux = edgeStates[1];
		edgeStates[1] = edgeStates[3];
		edgeStates[3] = edgeStates[2];
		edgeStates[2] = aux;
	}

	void BadOLLDeadGuy()
	{
		RotateAntiClockwise();
		PLLUaPerm();
		RotateClockwise();

		rotateCorner(cornerStates[0],1);
		rotateCorner(cornerStates[1],2);
		rotateCorner(cornerStates[2],1);
		rotateCorner(cornerStates[3],2);

		std::pair<int,int> aux=cornerStates[0];
		cornerStates[0]=cornerStates[1];
		cornerStates[1]=cornerStates[2];
		cornerStates[2]=cornerStates[3];
		cornerStates[3]=aux;

		aux = edgeStates[1];
		edgeStates[1] = edgeStates[3];
		edgeStates[3] = edgeStates[2];
		edgeStates[2] = aux;
	}

	void OLLHeadlights()
	{
		rotateCorner(cornerStates[2],1);
		rotateCorner(cornerStates[3],2);

		RotateAntiClockwise();
		PLLAaPerm();
		RotateClockwise();
	}

	void OLLT()
	{
		rotateCorner(cornerStates[0],1);
		rotateCorner(cornerStates[3],2);

		PLLAaPerm();
	}

	void OLLBowtie()
	{
		rotateCorner(cornerStates[0],1);
		rotateCorner(cornerStates[2],2);

		swap(cornerStates[0],cornerStates[2]);
		swap(cornerStates[1],cornerStates[3]);
	}
};

#define X_ALL_MOVES X(RotateClockwise) \
						X(RotateAntiClockwise) \
						X(OrientEdges) \
						X(PLLUaPerm) \
						X(PLLUbPerm) \
						X(PLLZPerm) \
						X(PLLHPerm) \
						X(PLLAaPerm) \
						X(PLLEPerm) \
						X(OLLSune) \
						X(OLLAntiSune) \
						X(OLLCross) \
						X(OLLDeadGuy) \
						X(OLLHeadlights) \
						X(OLLT) \
						X(OLLBowtie)

#define X_SUFFICIENT_MOVES X(RotateClockwise) \
						X(OrientEdges) \
						X(PLLAaPerm) \
						X(PLLZPerm) \
						X(PLLUaPerm)
