# #!/usr/bin/env python
# import os
# import sys

# from methods import print_error


# libname = "DunzemExtension"
# projectdir = "Arsend"

# localEnv = Environment(tools=["default"], PLATFORM="")

# # Build profiles can be used to decrease compile times.
# # You can either specify "disabled_classes", OR
# # explicitly specify "enabled_classes" which disables all other classes.
# # Modify the example file as needed and uncomment the line below or
# # manually specify the build_profile parameter when running SCons.

# # localEnv["build_profile"] = "build_profile.json"

# customs = ["custom.py"]
# customs = [os.path.abspath(path) for path in customs]

# opts = Variables(customs, ARGUMENTS)
# opts.Update(localEnv)

# Help(opts.GenerateHelpText(localEnv))

# env = localEnv.Clone()

# if not (os.path.isdir("godot-cpp") and os.listdir("godot-cpp")):
#     print_error("""godot-cpp is not available within this folder, as Git submodules haven't been initialized.
# Run the following command to download godot-cpp:

#     git submodule update --init --recursive""")
#     sys.exit(1)

# env = SConscript("godot-cpp/SConstruct", {"env": env, "customs": customs})

# env.Append(CPPPATH=["src/"])
# sources = Glob("src/*.cpp")


# if env["target"] in ["editor", "template_debug"]:
#     try:
#         doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
#         sources.append(doc_data)
#     except AttributeError:
#         print("Not including class reference as we're targeting a pre-4.3 baseline.")

# # .dev doesn't inhibit compatibility, so we don't need to key it.
# # .universal just means "compatible with all relevant arches" so we don't need to key it.
# suffix = env['suffix'].replace(".dev", "").replace(".universal", "")

# lib_filename = "{}{}{}{}".format(env.subst('$SHLIBPREFIX'), libname, suffix, env.subst('$SHLIBSUFFIX'))

# library = env.SharedLibrary(
#     "bin/{}/{}".format(env['platform'], lib_filename),
#     source=sources,
# )

# copy = env.Install("{}/bin/{}/".format(projectdir, env["platform"]), library)

# default_args = [library, copy]
# Default(*default_args)
#!/usr/bin/env python
import os
import sys

from methods import print_error

libname = "DunzemExtension"
projectdir = "Dunzem_Server"

localEnv = Environment(tools=["default"], PLATFORM="")

# Build profiles can be used to decrease compile times.
# You can either specify "disabled_classes", OR
# explicitly specify "enabled_classes" which disables all other classes.
# Modify the example file as needed and uncomment the line below or
# manually specify the build_profile parameter when running SCons.

# localEnv["build_profile"] = "build_profile.json"

customs = ["custom.py"]
customs = [os.path.abspath(path) for path in customs]

opts = Variables(customs, ARGUMENTS)
opts.Update(localEnv)

Help(opts.GenerateHelpText(localEnv))

env = localEnv.Clone()

if not (os.path.isdir("godot-cpp") and os.listdir("godot-cpp")):
    print_error("""godot-cpp is not available within this folder, as Git submodules haven't been initialized.
Run the following command to download godot-cpp:

    git submodule update --init --recursive""")
    sys.exit(1)

env = SConscript("godot-cpp/SConstruct", {"env": env, "customs": customs})

env.Append(CPPPATH=["src"])

# Recursively grab all .cpp files in src and subfolders, exclude generated files
sources = []
for root, dirs, files in os.walk("src"):
    for file in files:
        if file.endswith(".cpp"):
            full_path = os.path.join(root, file)
            # Exclude files in gen directory AND also exclude doc_data.gen.cpp specifically
            if "src/gen" not in full_path.replace("\\", "/") and not file == "doc_data.gen.cpp":
                sources.append(full_path)

# Debug: print all found sources
print("Found source files:")
for source in sources:
    print("  ", source)

if len(sources) == 0:
    print_error("No source files found! Trying alternative method...")
    # Try alternative glob method
    sources = []
    for pattern in ["src/*.cpp", "src/**/*.cpp", "src/**/**/*.cpp", "src/**/**/**/*.cpp"]:
        found = Glob(pattern)
        for f in found:
            f_str = str(f)
            if "src/gen" not in f_str and "doc_data.gen.cpp" not in f_str:
                sources.append(f)
    
    print("Alternative method found:")
    for source in sources:
        print("  ", source)

# Handle doc data generation - but only if we're not already finding it in sources
doc_data_node = None
if env["target"] in ["editor", "template_debug"]:
    try:
        # Check if doc_data.gen.cpp already exists in our sources
        doc_data_exists = any("doc_data.gen.cpp" in str(s) for s in sources)
        if not doc_data_exists:
            doc_data_node = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
            print("Generated doc_data.gen.cpp")
        else:
            print("doc_data.gen.cpp already found in sources, skipping generation")
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

# .dev doesn't inhibit compatibility, so we don't need to key it.
# .universal just means "compatible with all relevant arches" so we don't need to key it.
suffix = env['suffix'].replace(".dev", "").replace(".universal", "")

lib_filename = "{}{}{}{}".format(env.subst('$SHLIBPREFIX'), libname, suffix, env.subst('$SHLIBSUFFIX'))

# Build library - only include doc_data if it was generated
all_sources = sources
if doc_data_node:
    all_sources.append(doc_data_node)

library = env.SharedLibrary(
    "bin/{}/{}".format(env['platform'], lib_filename),
    source=all_sources,
)

# ✅ Only install the DLL (avoid .lib/.exp missing errors)
dll_only = library[0]
copy = env.Install("{}/bin/{}/".format(projectdir, env["platform"]), dll_only)

Default(dll_only, copy)