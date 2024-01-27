#!/usr/bin/env bash
set -e

function error() {
  echo >&2 "ERROR: ${1} Aborting." && false
}

function warning() {
  echo >&2 "WARNING: ${1}"
}

#[[ $# == 3 ]] || error "Expected exactly 3 parameters: '${0} <IMAGE_NAME> <IMAGE_VARIANT> <DOCKER_REPOSITORY>'."

IMAGE_NAME="${1}"
IMAGE_VARIANT="${2}"
DOCKER_REPOSITORY="${3}"
CONTAINER_NAME="${4}"
DOCKERFILE="scripts/docker/${IMAGE_NAME}/Dockerfile.${IMAGE_VARIANT}"

#echo "-- check_dockerfile_was_changed"

# exit, if the dockerfile was not changed.
#if git diff --quiet origin/develop HEAD -- "${DOCKERFILE}"; then
#  echo "${DOCKERFILE} was not changed. Nothing to do."
#  exit 0
#fi


echo "-- build_docker"

# This is a workaround: we run `docker build` twice to prevent the `layer does not exist` problem.
# See https://github.com/moby/moby/issues/37965.
docker build "scripts/docker/${IMAGE_NAME}" --file "scripts/docker/${IMAGE_NAME}/Dockerfile.${IMAGE_VARIANT}" --tag "${IMAGE_NAME}" ||
  docker build "scripts/docker/${IMAGE_NAME}" --file "scripts/docker/${IMAGE_NAME}/Dockerfile.${IMAGE_VARIANT}" --tag "${IMAGE_NAME}"

#echo "-- test_docker @ '${PWD}'"

#docker run --rm --name ${CONTAINER_NAME} --volume "${PWD}:/root/project" "${IMAGE_NAME}" "/root/project/scripts/ci/${IMAGE_NAME}_test_${IMAGE_VARIANT}.sh"
