$ErrorActionPreference = "Stop"

# Needed for Invoke-WebRequest to work via CI.
$progressPreference = "silentlyContinue"

$INSTALL_PATH = "$PSScriptRoot\..\build\deps\boost"
$NEWER_VER = "1.78.0"
$NEWER_VERSION = "1_78_0"
$BUILD_VER = "1.76.0"
$BUILD_VERSION = "1_76_0"

$NEWER_FILE_PATH = "boost_${NEW_VERSION}\tools\build\src"
$BUILD_FILE_PATH = "boost_${BUILD_VERSION}\tools\build\src"

if ( -not (Test-Path $INSTALL_PATH) ) {
  New-Item -ItemType Directory -Force -Path $INSTALL_PATH

  Invoke-WebRequest -URI "https://boostorg.jfrog.io/artifactory/main/release/${BUILD_VER}/source/boost_${BUILD_VERSION}.7z" -OutFile boost_${BUILD_VERSION}.7z
  Invoke-WebRequest -URI "https://boostorg.jfrog.io/artifactory/main/release/${NEWER_VER}/source/boost_${NEWER_VERSION}.7z" -OutFile boost_${NEWER_VERSION}.7z
  Copy-Item -Path "boost_${NEWER_VERSION}\bootstrap.bat" -Destination "boost_${BUILD_VERSION}\bootstrap.bat"
  Copy-Item -Path "${NEWER_FILE_PATH}\engine\build.bat" -Destination "${BUILD_FILE_PATH}\engine\build.bat"
  Copy-Item -Path "${NEWER_FILE_PATH}\engine\config_toolset.bat" -Destination "${BUILD_FILE_PATH}\engine\config_toolset.bat"
  Copy-Item -Path "${NEWER_FILE_PATH}\engine\guess_toolset.bat" -Destination "${BUILD_FILE_PATH}\engine\guess_toolset.bat"
  Copy-Item -Path "${NEWER_FILE_PATH}\engine\vswhere_usability_wrapper.cmd" -Destination "${BUILD_FILE_PATH}\engine\vswhere_usability_wrapper.cmd"
  Copy-Item -Path "${NEWER_FILE_PATH}\tools\msvc.jam" -Destination "${BUILD_FILE_PATH}\engine\msvc.jam"
  7z x boost_${BUILD_VERSION}.7z -o".\" -y
  7z x boost_${NEWER_VERSION}.7z -o".\" -y
  cd boost_${BUILD_VERSION}
  .\bootstrap.bat
  .\b2 -j4 -d0 link=static runtime-link=static variant=release threading=multi address-model=64 --with-filesystem --with-system --with-program_options --with-test --prefix=$INSTALL_PATH install
  if ( -not $? ) { throw "Error building boost." }
}
"BOOST_VER=$BUILD_VER" | Out-File -Append -FilePath $Env:GITHUB_ENV
