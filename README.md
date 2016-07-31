# CC32xx-bsd-wrapper
A more strictly complient BSD socket layer wrapper for the TI CC31xx and CC32xx SimpleLink API's

# Including in your project
To include this wrapper in your project, add the cc32xx-bsd-wrapper/include directory to your compilers included path ahead of any standard library includes.  Next add bsd_socket.c and bsd_select.c to your project build, and the resulting build artifacts to your final link.

In order for the project to build successfully, we must override the user.h that is located at cc3200-sdk/simplelink/user.h.  Remove or rename this file so that it does not get pulled in by your build, and cc32xx-bsd-wrapper/user.h gets pulled in instead.

# Example GCC build
There is an example build for the ARM GCC compiler.  Variables are set in path.mk in order to locate the CC3200SDK and the ARM compiler.  To execute the build, update the paths in path.mk to match your setup and type make on the command line in cc32xx-bsd-wrapper/ directory
