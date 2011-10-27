#FILE(GLOB headers *.h)
#FILE(GLOB sources *.cpp)

SET(headers
Camera.h
Coordinate2DHashTable.h
FASTFeatureTracker.h
FASTFinder.h
Feature.h
FeatureExtractor.h
FeatureFinder.h
FeatureMatcher.h
FeatureTracker.h
Frame.h
HashTable.h
ImagePatch.h
ImagePatchExtractor.h
Radius2DHashMatcher.h
Map.h
Point.h
polynomial.h
Projection.h
Timer.h
)

SET(sources
Camera.cpp
Coordinate2DHashTable.cpp
FASTFeatureTracker.cpp
FASTFinder.cpp
five_point.cpp
Frame.cpp
ImagePatch.cpp
ImagePatchExtractor.cpp
Radius2DHashMatcher.cpp
Map.cpp
Point.cpp
polynomial.cpp
Projection.cpp
Timer.cpp
main.cpp
)

ADD_EXECUTABLE(test ${sources} ${headers})

CUDA_ADD_EXECUTABLE(gpu_test main.cu Timer.cpp Timer.h)