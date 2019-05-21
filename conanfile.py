from conans import ConanFile, CMake

class OsgVisualizationConan(ConanFile):
    name = "OsgVisualization"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "cmake/*", "include/*", "src/*", "ui/*"
    requires = "Parametrizable/1.0@MatthiasMichael/stable", "Roi3DF/1.0@MatthiasMichael/stable", "openscenegraph/3.6.3@bincrafters/stable"
    options = { 
        "build_example_displayable": [True, False],
        "build_example_display_descriptor": [True, False]
    }
    default_options = {
        "build_example_displayable": False,
        "build_example_display_descriptor": False
    }

    def _configure_cmake(self):
        cmake = CMake(self)
        if not self.options.build_example_displayable and not self.options.build_example_display_descriptor:
            cmake.definitions["BUILD_EXAMPLES"] = False
        else:
            cmake.definitions["BUILD_EXAMPLES"] = True
            cmake.definitions["BUILD_EXAMPLE_DISPLAYABLE"] = self.options.build_example_displayable
            cmake.definitions["BUILD_EXAMPLE_DISPLAY_DESCRIPTOR"] = self.options.build_example_display_descriptor
        cmake.configure(source_dir="%s/cmake" % self.source_folder)
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def imports(self):
        if self.options.build_example_displayable:
            self.copy("*.dll", "test/test_displayable", "bin")
        if self.options.build_example_display_descriptor:
            self.copy("*.dll", "test/test_display_descriptor", "bin")

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("ui_*.h", dst="include", keep_path=False)
        self.copy("*.lib", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = [self.name]
