# JS RMG Object Extender

This standalone Visual Studio solution contains only `Objects_JSRMGObjectsExtender` and a local snapshot of the headers it needs. It does not depend on the original `h3-plugins-daemon-upsteam` checkout or its repository-level MSBuild files.

## Build

Open `Objects_JSRMGObjectsExtender.sln` and build **Win32** / **Debug** or **Release**. The project requires Visual Studio's C++ desktop workload and a Windows 10 SDK. The `.era` output and intermediate files go into the solution's `bin` and `obj` directories.

The bundled `headers` directory contains the H3API, ERA and object-extender API headers used by this project. To consume newer API changes, update that local snapshot deliberately.

Install the generated `Objects_JSRMGObjectsExtender.era` alongside the `RMG_CustomizeObjectProperties.era` provider plugin in the ERA plugins directory.