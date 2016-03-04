#==============================================================================
# Copyright (c) 2015 Frozen Team.
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation 
# the rights to use, copy, modify, merge, publish, distribute, sublicense, 
# and/or sell copies of the Software, and to permit persons to whom the 
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included 
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
# SOFTWARE.
#==============================================================================
#
# Locate GLI library
# This module defines:
# GLI_FOUND, if true, library is found
# GLI_INCLUDE_DIR, directory path where to find
# 	GLI library directory with all necessary headers.
# 
# $GLI_ROOT is an environment variable that
# would correspond to the installation path.
#
# TODO: Test it under OSX, Linux

set(GLI_PATHS ${GLI_ROOT} $ENV{GLI_ROOT})

find_path(GLI_INCLUDE_DIR gli/gli.hpp
	PATH_SUFFIXES include/gli
	PATHS ${GLI_PATHS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(GLI REQUIRED_VARS GLI_INCLUDE_DIR)