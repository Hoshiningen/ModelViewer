# Model Viewer

Experiments with procedural geometry.

## Building

You'll need to use/create a conan profile that defines the settings used to build the project, then install the conan dependencies and build with the following commands:

```
conan install <Path To conanfile.py> --profile <Path to Profile> -s build_type=Debug --build=missing
conan install <Path To conanfile.py> --profile <Path to Profile> -s build_type=Release --build=missing
conan build <Path To conanfile.py>
```

A sample Windows, Visual Studio profile is:

```toml
[settings]
    os=Windows
    arch=x86_64
    compiler=Visual Studio
    compiler.version=17
    compiler.cppstd=20
    compiler.toolset=v143
[options]
[build_requires]
[env]
```

See: [https://docs.conan.io/en/latest/using_packages/using_profiles.html](https://docs.conan.io/en/latest/using_packages/using_profiles.html) for more information on conan profiles.
