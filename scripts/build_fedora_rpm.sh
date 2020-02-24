# This script is used to build an RPM on Fedora with no rpmlint errors.
# By including a man page, the system scripts will add the man directories to the specs' files section.

# TODO: Add shebang

# TODO: Review remaining rpmlint warnings

# TODO: Check for tools used
# TODO: Improve structure
# TODO: Consider adding error codes
# TODO: Consider adding man page

# TODO: Check command outputs
set -e
unset IFS
# TODO: Default spec template doesn't exist on second run.
rm -rf custom_rpm_build
mkdir -p custom_rpm_build
cd custom_rpm_build
cmake .. -D 'CPACK_GENERATOR=RPM'
make
make package
spec_folder=_CPack_Packages/Linux/RPM/SPECS
spec_template=imagecrypt.spec.in
cp $spec_folder/$spec_template $spec_template
IFS=$'\n'
man_directories=($(grep -h '%dir.*man' $spec_folder/imagecrypt.spec))
unset IFS
for spec_line in "${man_directories[@]}"; do
	sed --in-place "/%files/a %exclude $spec_line" $spec_template
done

# TODO: Set CPACK_GENERATOR in CMakeLists.txt
cmake .. -D "CPACK_RPM_USER_BINARY_SPECFILE=$spec_template" -D 'CPACK_GENERATOR=RPM'
make
make package
rpmlint --info *.rpm