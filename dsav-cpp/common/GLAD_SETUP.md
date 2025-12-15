# GLAD Setup Instructions

GLAD (OpenGL Loader) files need to be generated and placed in this directory.

## Generating GLAD Files

1. Visit https://glad.dav1d.de/
2. Configure with the following settings:
   - **Language**: C/C++
   - **API gl**: Version 3.3 (or higher)
   - **Profile**: Core
   - **Extensions**: (leave default or add specific ones if needed)
   - **Options**: Check "Generate a loader"

3. Click "GENERATE" and download the zip file

4. Extract the contents and copy:
   - `include/glad/glad.h` → `dsav-cpp/common/include/glad/glad.h`
   - `include/KHR/khrplatform.h` → `dsav-cpp/common/include/KHR/khrplatform.h`
   - `src/glad.c` → `dsav-cpp/common/src/glad.c`

## Alternative: Use the provided script

```bash
cd dsav-cpp/common
# Script to download and setup GLAD (if available)
# ./setup_glad.sh
```

## Current Status

⚠️ **GLAD files are NOT yet generated.** You must complete the above steps before building the project.

Once GLAD is set up, the CMakeLists.txt is already configured to compile it.
