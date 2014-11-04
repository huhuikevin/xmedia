LOCAL_PATH:= $(call my-dir)
common_SRC_FILES := \
	udf25.cpp \
	UDFDirectory.cpp \
	UDFFile.cpp \
	URL.cpp	

common_C_INCLUDES := $(LOCAL_PATH)/include
	

# For the device
# =====================================================

include $(CLEAR_VARS)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include
LOCAL_SRC_FILES := $(common_SRC_FILES)
LOCAL_C_INCLUDES += $(common_C_INCLUDES)
#LOCAL_SHARED_LIBRARIES += libdl
LOCAL_CFLAGS += -DHAVE_CONFIG_H

LOCAL_MODULE:= libxbmc
include $(BUILD_SHARED_LIBRARY)


