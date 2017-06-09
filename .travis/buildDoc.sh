#!/usr/bin/env bash

if [ "${FLAGS}" == "-DCREATE_DOC=ON" ]; then
	cmake "${FLAGS}" ..
	cd $TRAVIS_BUILD_DIR
    chmod +x createAndDeployDocumentation.sh
    ./createAndDeployDocumentation.sh
fi
