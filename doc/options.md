# CMake options for configuring the library

When installing the library, each build type (`-DCMAKE_BUILD_TYPE=XXX`) allows a separate configuration.
The configuration doesn't affect your targets, you can link to any configuration (your compiler might complain though, if you link your debug target to a library compiled in release mode or vice-versa).

The build types `Debug`, `RelWithDebInfo` and `Release` provide pre-defined values for the debugging options,
those cannot be overridden.

You can create as many build types as you want.

## Options

There are the following variables available to configure it:

* `COMP_HAS_*`: specifies compatibility options, that is, whether a certain C++ feature is available under your compiler. They are automatically detected by CMake, so there is usually no need to change them.

* `FOONATHAN_MEMORY_BUILD_EXAMPLES/_TESTS`: whether or not to build examples or tests. If this is `OFF` their CMake scripts are not even included. It is `ON` for standalone builds and `OFF` if used in `add_subdirectory()`.
* `FOONATHAN_MEMORY_BUILD_TOOLS`: whether or not build the tools. Unlike the other two options, this is always `ON`.

* `FOONATHAN_MEMORY_EXTERN_TEMPLATE`: If active the library provides already the definition of common instantiations of its class templates. This can speed up compilation time of user code since the compiler does not need to generate the definition each time the class instantiation is used (this compilation time is done when compiling the library and the size of the library binary increases). Default is `ON`.

* `FOONATHAN_MEMORY_DEFAULT_ALLOCATOR`: The default allocator used by the higher level allocator classes. One of the low level allocators (see \ref foonathan::memory::default_allocator). Default is `heap_allocator`.
* `FOONATHAN_MEMORY_THREAD_SAFE_REFERENCE`: Whether or not the `allocator_reference` is thread safe by default. Default is `ON`.
* `FOONATHAN_MEMORY_MEMORY_RESOURCE` and `FOONATHAN_MEMORY_MEMORY_RESOURCE_HEADER`: Specifye th memory resource class used. The first one is the full name including namespace, the second one the header (including `"..."` or `<...>` that needs to be included for it. Default is `foonathan_comp::memory_resource` and `"foonathan/memory_resource.hpp"` to use the version of foonathan/compatibility.
* `FOONATHAN_MEMORY_TEMPORARY_STACK_MODE`: The `temporary_allocator` uses a `temporary_stack` for its allocation.
This option controls how and if a global, per-thread instance of it is managed.
If `2` it is automatically managed and created on-demand, if `1` you need explicit lifetime control through the `temporary_stack_initializer` class and if `0` there is no stack created automatically.
Mode `2` has a slight runtime overhead.

* `FOONATHAN_MEMORY_DEBUG_*`: Specifies debugging options such as pointer check in `deallocate()` or filling newly allocated memory with values. They are set automatically for certain build types and cannot be overriden: All of them are enabled in `Debug` builds, the faster ones in `RelWithDebInfo` and none in `Release`. See [debugging](md_doc_debug_error.html#debugging) for a detailed description.

A list of all options with description is generated by calling `cmake -LH`.

## Variables and targets

The following variables or targets are available if used with `add_subdirectory()`:

* `FOONATHAN_MEMORY_INCLUDE_DIR` (variable): The include directory for the header files.
* `FOONATHAN_MEMORY_VERSION[_MAJOR/MINOR]` (variable): Major and minor version of the library.
* `foonathan_memory` (target): The target of the library you can link to.
* `foonathan_memory_example_*` (target): The targets for the examples. Only available if `FOONATHAN_MEMORY_BUILD_EXAMPLES` is `ON`.
* `foonathan_memory_test` (target): The test target. Only available if `FOONATHAN_MEMORY_BUILD_TESTS` is `ON`.
* `foonathan_memory_profiling` (target): The profiling target. Only available if `FOONATHAN_MEMORY_BUILD_TESTS` is `ON`.
* `foonathan_memory_node_size_debugger` (target): The target that generates the container node size information. Only available if `FOONATHAN_MEMORY_BUILD_TOOLS` is `ON`.

Also every function from [foonathan/compatibility] is exposed.

[foonathan/compatibility]: https://github.com/foonathan/compatiblity