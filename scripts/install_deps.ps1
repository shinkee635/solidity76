$ErrorActionPreference = "Stop"

# Needed for Invoke-WebRequest to work via CI.
$progressPreference = "silentlyContinue"

$INSTALL_PATH = "$PSScriptRoot\..\build\deps\boost"

if ( -not (Test-Path $INSTALL_PATH) ) {
  New-Item -ItemType Directory -Force -Path $INSTALL_PATH

  Invoke-WebRequest -URI "https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.7z" -OutFile boost_1_78_0.7z
  #Invoke-WebRequest -URI "https://boostorg.jfrog.io/artifactory/main/release/1.84.0/source/boost_1_84_0.zip"
  #tar -xf boost_1_84_0.zip
  7z x boost_1_78_0.7z -o".\" -y
  cd boost_1_78_0
  .\bootstrap.bat
  .\b2 -j4 -d0 link=static runtime-link=static variant=release threading=multi address-model=64 --with-filesystem --with-system --with-program_options --with-test --prefix=$INSTALL_PATH install
  if ( -not $? ) { throw "Error building boost." }
  cd ..
}
