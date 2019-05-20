from conans import ConanFile, CMake

class OsgCommons_Test_DisplayDescriptorConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    requires = "Parametrizable/1.0@MatthiasMichael/stable", "Roi3DF/1.0@MatthiasMichael/stable", "openscenegraph/3.6.3@bincrafters/stable"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_dir="%s/cmake" % self.source_folder)
        cmake.build()

    def imports(self):
        self.copy("*.dll", "", "bin")