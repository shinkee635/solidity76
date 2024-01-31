$ErrorActionPreference = "Stop"

$githubWorkspace = $Env:GITHUB_WORKSPACE
# Needed for Invoke-WebRequest to work via CI.
$progressPreference = "silentlyContinue"

mkdir build\deps\z3 -Force
Invoke-WebRequest -URI "https://github.com/Z3Prover/z3/archive/refs/tags/z3-4.8.9.zip" -OutFile "z3.zip"
Expand-Archive -LiteralPath z3.zip -DestinationPath .\
cd z3-z3-4.8.9
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -DZ3_BUILD_LIBZ3_SHARED=OFF -DCMAKE_INSTALL_PREFIX="$githubWorkspace\build\deps\z3" -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release --target libz3 -j 12
cmake --install .

# Export to environment variable
"Z3_DIR=$githubWorkspace\build\deps\z3\lib\cmake\z3" | Out-File -Append -FilePath $Env:GITHUB_ENV
cd ..\..
