#!/bin/bash

# Returns the number of commits since previous release/tag to act as a build number.

# (Optional) provide current version.
VERSION=$1

# If current version was not set, pick latest tag.
if [ -z ${VERSION} ]
then
	PREVIOUS=$(git describe --abbrev=0 --tags 2> /dev/null)
else
	# Pick previous tag.
	PREVIOUS=$(git tag -l --sort=v:refname | grep -v "$VERSION" | tail -n 1)
fi

# If no previous tag was found, set PREVIOUS to root.
if [ -z ${PREVIOUS} ]
then
	PREVIOUS=$(git log --oneline --pretty=format:"%h" | tail -1)
fi

# Number of commits between PREVIOUS and HEAD.
echo $(git rev-list ${PREVIOUS}..HEAD --count)
