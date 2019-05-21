from conans import ConanFile, CMake

class OsgVisualizationConan(ConanFile):
    name = "OsgVisualization"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "cmake/*", "include/*", "src/*", "ui/*"
    requires = "Parametrizable/1.0@MatthiasMichael/stable", "Roi3DF/1.0@MatthiasMichael/stable", "openscenegraph/3.6.3@bincrafters/stable"

    def build(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_EXAMPLES"] = False
        cmake.configure(source_dir="%s/cmake" % self.source_folder)
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("ui_*.h", dst="include", keep_path=False)
        self.copy("*.lib", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = [self.name]
