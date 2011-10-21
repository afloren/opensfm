#file(GLOB headers *.h)
#file(GLOB sources *.cpp)

set(headers
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
ImagePatchMatcher.h
KDTree.h
Map.h
Point.h
Projection.h
Timer.h
)

set(sources
Camera.cpp
Coordinate2DHashTable.cpp
FASTFeatureTracker.cpp
FASTFinder.cpp
Frame.cpp
ImagePatchExtractor.cpp
ImagePatchMatcher.cpp
Map.cpp
Point.cpp
Projection.cpp
Timer.cpp
main.cpp
)

add_executable(test ${sources} ${headers})