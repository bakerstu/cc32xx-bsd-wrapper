# CC32xx-bsd-wrapper
A more strictly complient BSD socket layer wrapper for the TI CC31xx and CC32xx SimpleLink API's

## What does "more strictly" mean?
The TI CC3x SimpleLink BSD-like API's (socket, bind, listen, accept, connect, etc...) convey error conditions directly with their return values.  The BSD convention is that an API call resulting in an error returns -1 and provides the specific error condition in the global "errno" variable.  While the CC3x SimpleLink BSD-like API's can be somewhat simpler to handle for new code, it remains a challenge to resuse existing BSD compliant code that was origonally written for, or is to be shared with, another OS target such as eCos, Linux, Mac OS X, QNX, VxWorks, Windows, etc...

This project makes a best effort attempt to wrap the CC3x SimpleLink BSD-like API's to be more complient with the BSD conventions.  The project also makes a best effort to provide conventional header files as one would typically find on a traditional Unix system.

# Including in your project
To include this wrapper in your project, add the cc32xx-bsd-wrapper/include directory to your build include path ahead of any standard library includes.  Next add bsd_socket.c and bsd_select.c to your project build, and the resulting build artifacts to your final link.  If you are using an RTOS, be sure that you have a thread safe errno implementation.

In order for the project to build successfully, we must override the user.h that is located at cc3200-sdk/simplelink/user.h.  Remove or rename this file so that it does not get pulled in by your build, and cc32xx-bsd-wrapper/user.h gets pulled in instead.

# Example GCC build
There is an example build for the ARM GCC compiler.  Variables are set in path.mk in order to locate the CC3200SDK and the ARM compiler.  To execute the build, update the paths in path.mk to match your setup and type make on the command line in cc32xx-bsd-wrapper/ directory.
