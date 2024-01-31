$ErrorActionPreference = "Stop"

# Needed for Invoke-WebRequest to work via CI.
$progressPreference = "silentlyContinue"

Invoke-WebRequest -URI "https://github.com/Z3Prover/z3/releases/download/z3-4.8.9/z3-4.8.9-x64-win.zip" -OutFile "z3.zip"
mkdir deps
tar -xf z3.zip "deps"
