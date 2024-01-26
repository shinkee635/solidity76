#!/usr/bin/env bash
set -e

function error() {
  echo >&2 "ERROR: ${1} Aborting." && false
}

function warning() {
  echo >&2 "WARNING: ${1}"
}

[[ $# == 3 ]] || error "Expected exactly 3 parameters: '${0} <IMAGE_NAME> <IMAGE_VARIANT> <DOCKER_REPOSITORY>'."

IMAGE_NAME="${1}"
IMAGE_VARIANT="${2}"
DOCKER_REPOSITORY="${3}"
DOCKERFILE="scripts/docker/${IMAGE_NAME}/Dockerfile.${IMAGE_VARIANT}"

echo "-- test_build @ '${PWD}'"

docker run --rm --volume "${PWD}:/root/project" "${IMAGE_NAME}" "/root/project/scripts/tests.sh"
