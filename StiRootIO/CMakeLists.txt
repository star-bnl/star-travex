root_generate_dictionary(
	StiRootIO_Cint
	H2D.h
	H3D.h
	Profile2D.h
	Profile3D.h
	StiTreeMaker.h
	TStiEvent.h
	TStiHit.h
	TStiHitProxy.h
	TStiKalmanTrack.h
	TStiKalmanTrackNode.h
	LINKDEF StiRootIOLinkDef.h
)

add_library(
	StiRootIO
	SHARED
	H2D.cxx
	H3D.cxx
	PrgOptionProcessor.cxx
	Profile2D.cxx
	Profile3D.cxx
	StiHistContainer.cxx
	StiRootFile.cxx
	StiTreeMaker.cxx
	TStiEvent.cxx
	TStiHit.cxx
	TStiHitProxy.cxx
	TStiKalmanTrack.cxx
	TStiKalmanTrackNode.cxx
	StiRootIO_Cint.cxx
)

install(TARGETS StiRootIO DESTINATION ".$ENV{STAR_HOST_SYS}/lib")
