#include "fvCFD.H"

int main(int argc, char *argv[])
{
	// Read
	Foam::argList args(argc,argv);
	if(not args.checkRootCase()) Foam::FatalError.exit();

	Foam::Time runTime(Foam::Time::controlDictName, args);

    Foam::fvMesh mesh
    (
        Foam::IOobject
        (
            Foam::fvMesh::defaultRegion,
            runTime.timeName(),
            runTime,
            Foam::IOobject::MUST_READ
        )
    );

	// Write
	
	labelList procIds(mesh.cells().size());

	forAll(mesh.cells(), cid){
		const vector &C = mesh.C()[cid];
		if(C[0]*C[0]+C[1]*C[1] < 0.01){
			procIds[cid] = 1;
		}
		else{
			procIds[cid] = 0;
		}
		if(C[0] + C[1] < 0.1){
			++procIds[cid];
		}
	}

	labelIOList cellDecomposition
	(
		IOobject
		(
			"cellDecomposition",
			mesh.facesInstance(),
			mesh,
			IOobject::NO_READ,
			IOobject::NO_WRITE,
			false
		),
		procIds
	);
	cellDecomposition.write();

	return 0;
}
