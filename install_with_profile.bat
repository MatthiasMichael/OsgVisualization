conan install . -if %1 -pr %2

set test_projects=^
test_displayable ^
test_displayDescriptor

(for %%a in (%test_projects%) do (
	IF NOT EXIST %1.test/%%a (
		mkdir %1.test/%%a
	)
	conan install test/%%a -if %1.test/%%a -p %2
))

mkdir %1
conan build . -bf %1 -c