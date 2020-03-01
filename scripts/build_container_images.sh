#!/bin/bash

# TODO: Add options for build-only and push-only

IMAGE_NAMES=('archlinux_latest'
             'centos_latest'
             'clearlinux_latest'
             'debian_latest'
             'fedora_latest'
             'mageia_7'
             'ubuntu_19_10'
             'ubuntu_20_04');

STAGES=('build' 'deploy')

# TODO: Source these from config
readonly REGISTRY_NAME='lptech1024/imagecrypt';
readonly REGISTRY='docker.io';

set_docker_tool()
{
	local container_tools=('podman' 'docker');
	for tool in "${container_tools[@]}"; do
		hash "$tool" 2> /dev/null;
		if [[ $? -eq 0 ]]; then
			container_tool=$tool;
			break;
		fi
	done
}

build_images()
{
	for image_name in "${IMAGE_NAMES[@]}"; do
		for stage in "${STAGES[@]}"; do
			${container_tool} build --tag ${REGISTRY_NAME}:${stage}-${image_name} --file containers/Dockerfile-${stage}-${image_name}
			if [[ $? -ne 0 ]]; then
				echo "Skipping remaining stage(s) for ${image_name}.";
				break;
			fi
		done
	done
}

push_images()
{
	for image_name in "${IMAGE_NAMES[@]}"; do
		for stage in "${STAGES[@]}"; do
			${container_tool} push ${REGISTRY_NAME}:${stage}-${image_name} ${REGISTRY_NAME}:${stage}-${image_name}
		done
	done
}

main()
{
	set_docker_tool;
	if [[ -z container_tool ]]; then
		echo "Container tool not found.";
		# TODO: Don't use generic, magic value
		exit 1;
	fi

	# TODO: Add config options
	${container_tool} login ${REGISTRY};

	build_images;

	# TODO: Only push images if login was successful
	push_images;
}

main;
