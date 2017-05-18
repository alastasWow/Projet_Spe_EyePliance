#find_library(MGL mgl PATHS /usr/local/lib)


# Exécutable "track", compilé à partir du fichier Face_EyeTracking.cpp
ADD_EXECUTABLE(track Face.cpp Face.hpp Eye.cpp Eye.hpp Pupil.cpp Pupil.hpp Filtre.cpp Filtre.hpp main.cpp)

# Track sera linké avec les bibliothèques d'OpenCV
TARGET_LINK_LIBRARIES(track ${OpenCV_LIBS} )
#target_link_libraries(track ${MGL})
