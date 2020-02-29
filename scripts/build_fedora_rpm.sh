#!/bin/bash

# This script is used to build an RPM on Fedora with no rpmlint errors.
# By including a man page, the system scripts will add the man directories to the specs' files section.

set -e

main()
{
	mkdir --parents custom_rpm_build
	cd custom_rpm_build

	# TODO: Set CPACK_GENERATOR in CMakeLists.txt
	cmake .. -D 'CPACK_RPM_USER_BINARY_SPECFILE=' -D 'CPACK_GENERATOR=RPM'
	make package

	local -r spec_folder='_CPack_Packages/Linux/RPM/SPECS'
	local -r spec_template='imagecrypt.spec.in'
	cp $spec_folder/$spec_template $spec_template

	IFS=$'\n'
	man_directories=($(grep -h '%dir.*man' $spec_folder/imagecrypt.spec))
	unset IFS

	for spec_line in "${man_directories[@]}"; do
		sed --in-place "/%files/a %exclude $spec_line" $spec_template
	done

	cmake .. -D "CPACK_RPM_USER_BINARY_SPECFILE=$spec_template"
	make package
	rpmlint --info *.rpm
}

main;
