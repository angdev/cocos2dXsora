# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# the purpose of this sample is to demonstrate how one can
# generate two distinct shared libraries and have them both
# uploaded in
#

LOCAL_PATH:= $(call my-dir)

# first lib, which will be built statically
#
include $(CLEAR_VARS)

LOCAL_MODULE := tinyxml_static

LOCAL_MODULE_FILENAME := libtinyxml

LOCAL_SRC_FILES := tinystr.cpp	\
	tinyxml.cpp	\
	tinyxmlerror.cpp	\
	tinyxmlparser.cpp

include $(BUILD_STATIC_LIBRARY)


# second lib, which will depend on and include the first one
#
include $(CLEAR_VARS)

LOCAL_MODULE := tinyxml_shared

LOCAL_MODULE_FILENAME := libtinyxml

LOCAL_SRC_FILES := tinystr.cpp	\
	tinyxml.cpp	\
	tinyxmlerror.cpp	\
	tinyxmlparser.cpp

include $(BUILD_SHARED_LIBRARY)
