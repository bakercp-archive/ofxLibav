A set of utilities for working with libav.

Since libav is a complex library, there is no attempt to wrap every feature for use in openFrameworks.  Rather, this addon is meant to make it easy to add libav libraries across platforms while wrapping some useful features, such as media file meta information, etc.


Notes:
- When using project generator with this addon on OS X, including the libavutil headers cause a conflict.  To resolve the conflict, manually remove links to the libavutil folder from both the project outline and the HEADER_SEARCH_PATHS in the project settings.  Additionall, you must remove the link to libavutil.a.  A solution for this issue is in the works ... 
- Libav static libs compiled with license: LGPL version 2.1 or later
